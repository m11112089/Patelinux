import os
import hashlib

def file_hash(filepath):
    """Generate a hash for a file."""
    hash_algo = hashlib.md5()
    with open(filepath, 'rb') as f:
        for chunk in iter(lambda: f.read(4096), b""):
            hash_algo.update(chunk)
    return hash_algo.hexdigest()

def find_common_files(folder1, folder2):
    """Find common files in two folders based on their hash."""
    folder1_files = {file_hash(os.path.join(folder1, f)): f for f in os.listdir(folder1) if os.path.isfile(os.path.join(folder1, f))}
    folder2_files = {file_hash(os.path.join(folder2, f)): f for f in os.listdir(folder2) if os.path.isfile(os.path.join(folder2, f))}
    
    common_files = set(folder1_files.keys()).intersection(folder2_files.keys())
    return [(os.path.join(folder1, folder1_files[hash]), os.path.join(folder2, folder2_files[hash])) for hash in common_files]

def delete_files(file_pairs):
    """Delete files given a list of file pairs."""
    for file1, file2 in file_pairs:
        os.remove(file1)
        #os.remove(file2)
        print(f"Deleted: {file1} and {file2}")

if __name__ == "__main__":
    folder1 = "mnt/py-env/usr/bin"
    folder2 = "mnt/rootfs/usr/bin"
    
    common_files = find_common_files(folder1, folder2)
    delete_files(common_files)
