import os
import argparse

def parallelize_csv(dataset_directory, base_file_name, num_parallel, has_header=False):
    """
    Splits a large CSV file into smaller files based on round-robin distribution.

    Args:
        dataset_directory (str): Path to the input CSV file directory.
        base_file_name (str): Name of the CSV file in the directory.
        num_parallel (int): Number of parallel files to create.
        has_header (bool): Whether the input file has a header row.
    """
    output_files = []
    ext = '.csv'
    base_dir = os.path.dirname(dataset_directory)
    dataset_name = os.path.basename(dataset_directory)
    input_file = os.path.join(base_dir, dataset_name, base_file_name)

    for i in range(num_parallel):
        output_directory = os.path.join(base_dir, f'{dataset_name}_{i+1:03d}')
        os.makedirs(output_directory, exist_ok=True)
        output_path = os.path.join(output_directory, base_file_name)
        output_files.append(open(output_path, 'w'))

    with open(input_file, 'r') as infile:
        if has_header:
            header = infile.readline()
            for outfile in output_files:
                outfile.write(header)

        for i, line in enumerate(infile):
            output_files[i % num_parallel].write(line)

    for outfile in output_files:
        outfile.close()

    print(f"✅ Split {input_file} into {num_parallel} files.")


def main():
    parser = argparse.ArgumentParser(description="Split a CSV file in round-robin fashion for parallel processing.")
    parser.add_argument("dataset_directory", help="Path to the dataset directory that contains the CSV file.")
    parser.add_argument("base_file_name", help="Name of the CSV file (e.g., edges.csv).")
    parser.add_argument("num_parallel", type=int, help="Number of output files / parallel processes.")
    parser.add_argument("--has_header", action="store_true", help="Use this flag if the CSV file has a header row.")

    args = parser.parse_args()
    parallelize_csv(args.dataset_directory, args.base_file_name, args.num_parallel, args.has_header)

if __name__ == "__main__":
    main()
