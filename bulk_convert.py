
import os
import subprocess 
import argparse 

def pnmtopng(file_name_no_ext):
  file_as_ppm = f"{file_name_no_ext}.ppm"
  file_as_png = f"{file_name_no_ext}.png"
  cmd = f"pnmtopng {file_as_ppm} > {file_as_png}"
  subprocess.run([cmd], shell=True)

def get_args():
  parser = argparse.ArgumentParser(
    prog="Bulk Converter",
    description="Converts outputted `.ppm` files to `.png` format and places in output directory using ffmpeg"
  )
  parser.add_argument(
    "-d", 
    "--directory", 
    help="Directory at which the root of the project is",
    default=None,
    type=str,
    required=False
  )
  args = parser.parse_args()
  return args

def get_directory(args):
  if args.directory is None:
    return os.getcwd()
  if os.path.exists(args.directory):
    return args.directory
  print(f"Supplied path '{args.directory}' is not a valid path, using current working directory")
  return os.getcwd()

def check_for_bin(dir):
  bin_path = os.path.join(dir, "build", "bin")
  if not os.path.exists(bin_path):
    raise Exception(f"Supplied path {dir} does not contain build/bin")
  return bin_path

def get_executables(bin_path):
  for (root,dirs,files) in os.walk(bin_path, topdown=True):
    return files
  
def run_exes(bin_loc, exes):
  for e in exes:
    if e[-4:] == ".pdb":
      continue
    run = os.path.join(bin_loc, e)
    print(f"Running: {run}")
    subprocess.run([run], shell=True)

def collect_ppms_no_ext():
  # always in the cwd it will get generated
  [_, _, files] = os.walk(os.getcwd(), topdown=True)
  return [f[:-4] for f in files if f[-4:] == ".ppm"]

def get_output(dir):
  output = os.path.join(dir, "output")
  if not os.path.exists(output):
    print(f"Couldn't find output, making {output}")
    subprocess.run(["mkdir", "-p", output], shell=True)
  return output
  

def move(png_no_ext, output):
  file_to_move = f"{png_no_ext}.png"
  new_out = os.path.join(output, file_to_move)
  cmd = f"mv {file_to_move} {new_out}"
  subprocess.run([cmd], shell=True)

def run_convert_move(bin_loc, output_loc, exe):
  # outputted ppm should be same name as exe
  if exe[-4:] == ".pdb":
    return
  run = os.path.join(bin_loc, exe)
  subprocess.run([run], shell=True)
  # file should be in cwd
  file_no_ext = exe[:-4] if exe[-4:] == ".exe" else exe
  pnmtopng(file_no_ext)
  move(file_no_ext, output_loc) 

def main():
  #parse directory
  args = get_args()
  d = get_directory(args)
  # ensure that build/bin exists
  bin_loc = check_for_bin(d)
  output_loc = get_output(d)
  # gather all executables
  exes = get_executables(bin_loc)
  for e in exes:
    run_convert_move(bin_loc, output_loc, e)


if __name__ == '__main__':
  main()