import struct
import json
import argparse
import os

_format_version_ = 2
_max_chance_ = 10000

def validate_color(color_entry):
    if color_entry is None or not isinstance(color_entry, list) or len(color_entry) != 3:
        return False
    
    return True

def write_info_block(file, info_json):
    if info_json is None:
        raise Exception("Info block is missing from JSON data.")

    cell_name = info_json['name'].encode('utf-8')
    cell_name_length = len(cell_name)

    if cell_name_length > 255:
        raise Exception("Cell name is too long.")

    block_data = struct.pack('H', _format_version_)
    block_data += struct.pack('B', cell_name_length) + cell_name
    block_data += struct.pack('B', info_json.get('display', True))
    block_data += struct.pack('B', info_json.get('solid', True))
    block_data += struct.pack('B', info_json.get('gravity', False))
    block_data += struct.pack('B', info_json.get('fluid', False))

    density = info_json.get('density', -1)
    if density < 0 or density > 65535:
        raise Exception("Density value is invalid.")
    block_data += struct.pack('H', density)

    color = info_json.get('color', None)
    if not validate_color(color):
        raise Exception("Color value is invalid.")
    block_data += struct.pack('B', color[0])
    block_data += struct.pack('B', color[1])
    block_data += struct.pack('B', color[2])

    color_variation = info_json.get('colorVariation', [0, 0, 0])
    if not validate_color(color_variation):
        raise Exception("Color variation value is invalid.")
    block_data += struct.pack('B', color_variation[0])
    block_data += struct.pack('B', color_variation[1])
    block_data += struct.pack('B', color_variation[2])

    file.write(b'INFO')
    file.write(struct.pack('I', len(block_data)))
    file.write(block_data)


def write_mixing_block(file, mixing_json):
    if mixing_json is None:
        return
    
    if not isinstance(mixing_json, list):
        raise Exception("Mixing block is invalid.")
    
    mix_count = len(mixing_json)
    if mix_count > 65535:
        raise Exception("Too many mixing entries.")

    block_data = struct.pack('H', mix_count)
    for mix_entry in mixing_json:
        if mix_entry is None or not isinstance(mix_entry, list):
            raise Exception("Mixing entry is invalid.")
        if len(mix_entry) < 3 or len(mix_entry) > 4:
            raise Exception("Mixing entry is wrong size. Expected 3 or 4 elements.")
        
        if len(mix_entry) == 3:
            mix_entry.append(False)
        
        trigger_name = mix_entry[0].encode('utf-8')
        trigger_name_length = len(trigger_name)
        if trigger_name_length > 255:
            raise Exception("Mixing trigger name is too long.")
        
        block_data += struct.pack('B', trigger_name_length) + trigger_name

        result_name = mix_entry[1].encode('utf-8')
        result_name_length = len(result_name)
        if result_name_length > 255:
            raise Exception("Mixing result name is too long.")
        
        block_data += struct.pack('B', result_name_length) + result_name

        chance = mix_entry[2]
        if chance < 0 or chance > _max_chance_:
            raise Exception("Mixing chance value is invalid.")
        
        block_data += struct.pack('H', chance)

        reverse = mix_entry[3] == True
        block_data += struct.pack('B', reverse)

    file.write(b'MIXS')
    file.write(struct.pack('I', len(block_data)))
    file.write(block_data)    


def process_file(input_path, output_path):
    if not input_path.endswith(".json"):
        print(f"Warning: {input_path} is not a JSON file. Skipping.")
        return
    
    input_file = open(input_path, 'r')
    if input_file is None:
        print(f"Error: Could not open {input_path}")
        return
    
    json_data = json.load(input_file)
    if json_data is None:
        print(f"Error: {input_path} is not a valid JSON file.")
        return

    file = open(output_path, 'wb')
    if file is None:
        print(f"Error: Could not open {output_path}")
        return
    
    print(f"Processing {input_path}...")
    
    try:
        file.write(b'CELLBF')
        file.write(b'\x00\x00\x00\x00') # Placeholder for block size

        write_info_block(file, json_data['info'])
        write_mixing_block(file, json_data.get('mixing', None))

        file_size = file.tell() - 10
        file.seek(6)
        file.write(struct.pack('I', file_size))

    except Exception as e:
        print(f"Error: {e}")
        return


def __main__():
    parser = argparse.ArgumentParser(description="Convert JSON data to CELL binary format")
    parser.add_argument("input_dir", type=str, help="Input directory containing JSON files")
    parser.add_argument("output_dir", type=str, help="Output directory to write binary files")
    args = parser.parse_args()

    if not os.path.exists(args.input_dir):
        print(f"Error: {args.input_dir} does not exist.")
        return
    
    if not os.path.isdir(args.input_dir):
        print(f"Error: {args.input_dir} is not a directory.")
        return
    
    if not os.path.exists(args.output_dir):
        print(f"Warning: {args.output_dir} does not exist. Creating directory.")
        os.makedirs(args.output_dir)

    if not os.path.isdir(args.output_dir):
        print(f"Error: {args.output_dir} is not a directory.")
        return
    
    for file in os.listdir(args.input_dir):
        input_file = os.path.join(args.input_dir, file)
        output_file = os.path.join(args.output_dir, file.replace(".json", ".cell"))
        process_file(input_file, output_file)


    print("Done")


if __name__ == "__main__":
    __main__()
