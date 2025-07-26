import os
import argparse

def recombine_csv(base_directory, dataset_name, num_parts, block_size, has_header=False):
    """
    Recombines block-wise split CSV files back into a single CSV file
    while preserving the original edge order based on (block-wise, sequential assignment).

    Args:
        base_directory (str): Directory containing the output part files.
        dataset_name (str): Name of the dataset (used in filenames).
        num_parts (int): Number of part files (equal to number of workers).
        block_size (int): Size of each block assigned to a worker (p).
        has_header (bool): Whether the files have a header row.
    """

    # Step 1: Load all files into memory
    all_lines = [[] for _ in range(num_parts)]
    for i in range(num_parts):
        part_path = os.path.join(base_directory, f"anoedge_g_{dataset_name}_{i+1:03d}_score.csv")
        with open(part_path, "r") as f:
            lines = f.readlines()
            if has_header:
                lines = lines[1:]
            all_lines[i] = lines

    # Step 2: Reconstruct global order using block assignment rule
    total_lines = sum(len(lines) for lines in all_lines)
    recombined = []

    k = num_parts  # number of workers
    p = block_size  # block size

    for i in range(total_lines):
        block_index = i // p
        worker_id = block_index % k
        index_in_file = (block_index // k) * p + (i % p)

        if index_in_file < len(all_lines[worker_id]):
            recombined.append(all_lines[worker_id][index_in_file])
        else:
            # In case of uneven splits or padding
            continue

    # Step 3: Write final recombined file
    output_path = os.path.join(base_directory, f"anoedge_g_{dataset_name}_score.csv")
    with open(output_path, "w") as out:
        if has_header:
            out.write("node1,node2,timestamp,score\n")  # or use dynamic header detection if needed
        out.writelines(recombined)

    print(f"✅ Recombined file written to: {output_path}")


def main():
    parser = argparse.ArgumentParser(description="Recombine round-robin split CSV files into a single CSV.")
    parser.add_argument("base_directory", help="Path to the parent directory where split folders exist.")
    parser.add_argument("dataset_name", help="Base name of the dataset (used to find split folders).")
    parser.add_argument("num_parts", type=int, help="Number of split parts to combine.")
    parser.add_argument("block_size", type=int, help="Block size (p) used in block-wise splitting.")
    parser.add_argument("--has_header", action="store_true", help="Use this flag if CSV files have a header row.")

    args = parser.parse_args()
    recombine_csv(args.base_directory, args.dataset_name, args.num_parts, args.block_size, args.has_header)


if __name__ == "__main__":
    main()
