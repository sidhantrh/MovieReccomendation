# MovieMatch

MovieMatch is a C++ program that recommends movies based on similarity to a movie selected by the user. It compares recommendation ranking performance using a Max Heap and a Red-Black Tree.

## How to Run

1. Download the IMDb non-commercial dataset files from:  
   https://developer.imdb.com/non-commercial-datasets/

2. Unzip the files and place these `.tsv` files in the project folder:
   - `title.basics.tsv`
   - `title.ratings.tsv`
   - `name.basics.tsv`
   - `title.principals.tsv`
   - `title.akas.tsv`

3. Open the project in CLion or another C++ IDE.
4. Make sure the working directory is set to the project folder.
5. Build and run `main.cpp`.

## Notes

The `.tsv` dataset files are not included in this GitHub repository because they are too large. The program must be run with the required IMDb files placed in the project folder.
