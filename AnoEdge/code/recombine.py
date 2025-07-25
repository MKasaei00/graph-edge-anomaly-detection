import os
import argparse

def recombine_csv(base_directory, dataset_name, num_parts, has_header=False):
    """
    Recombines round-robin split CSV files back into one file.

    Args:
        base_directory (str): Parent directory where dataset splits exist.
        dataset_name (str): Name of the dataset (used for locating split folders).
        num_parts (int): Number of parts that were created during the split.
        has_header (bool): Whether the original files have a header row.
    """
    part_files = []
    for i in range(num_parts):
        part_path = os.path.join(base_directory, f"anoedge_g_{dataset_name}_{i+1:03d}_score.csv")
        part_files.append(open(part_path, "r"))

    output_path = os.path.join(base_directory, f"anoedge_g_{dataset_name}_score.csv")
    with open(output_path, "w") as outfile:
        # Handle header row
        if has_header:
            header = part_files[0].readline()
            outfile.write(header)
            for f in part_files[1:]:
                _ = f.readline()  # Skip header line

        while True:
            lines_written = 0
            for f in part_files:
                line = f.readline()
                if line:
                    outfile.write(line)
                    lines_written += 1
            if lines_written == 0:
                break

    for f in part_files:
        f.close()

    print(f"✅ Recombined file written to: {output_path}")


def main():
    parser = argparse.ArgumentParser(description="Recombine round-robin split CSV files into a single CSV.")
    parser.add_argument("base_directory", help="Path to the parent directory where split folders exist.")
    parser.add_argument("dataset_name", help="Base name of the dataset (used to find split folders).")
    parser.add_argument("num_parts", type=int, help="Number of split parts to combine.")
    parser.add_argument("--has_header", action="store_true", help="Use this flag if CSV files have a header row.")

    args = parser.parse_args()
    recombine_csv(args.base_directory, args.dataset_name, args.num_parts, args.has_header)


if __name__ == "__main__":
    main()
