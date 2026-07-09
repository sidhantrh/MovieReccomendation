//
// Created by sidha on 7/2/2026.
#include "movie.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

vector<movie> loadMovies(const string& filename)
{
    vector<movie> movies;

    // open file
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: could not open file " << filename << endl;
        return movies;
    }

    string line;

    // Skip header line
    getline(file, line);

    cout << "loadMovies called with: " << filename << endl;
    int count = 0;

    // read each line
    while (getline(file, line)) {
        count++;

        if (count % 100000 == 0) {
            cout << "Processed " << count << " lines..." << endl;
        }

        if (count >= 10000) {
            break;
        }
        vector<string> tokens = split(line, '\t');

        if (tokens.size() < 9) {
            continue;
        }

        string titleType = tokens[1];

        // Only keep actual movies
        if (titleType != "movie") {
            continue;
        }

        // Skip missing year/runtime
        if (tokens[5] == "\\N" || tokens[7] == "\\N") {
            continue;
        }

        movie Movie;

        Movie.id = tokens[0];
        Movie.title = tokens[2];
        Movie.year = stoi(tokens[5]);
        Movie.runtime = stoi(tokens[7]);
        Movie.genres = split(tokens[8], ',');
        Movie.rating = 0.0;
        Movie.numVotes = 0;


        movies.push_back(Movie);
    }

    file.close();

    return movies;
}




vector<string> split(const string& str, char delimiter)
{
    vector<string> result;

    stringstream ss(str);

    string token;

    while(getline(ss, token, delimiter))
        result.push_back(token);

    return result;
}

vector<string> splitGenres(const string& genres) {
    if (genres == "\\N") return {};
    return split(genres, ',');
}

unordered_map<string,movie>loadratings(const string& filename, vector<movie> movies){
    unordered_map<string, movie> rating_by_movie;

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: could not open file " << filename << endl;
        return rating_by_movie;
    }

    // First, put all existing movies into the map by id
    for (movie m : movies) {
        rating_by_movie[m.id] = m;
    }

    string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        vector<string> tokens = split(line, '\t');

        if (tokens.size() < 3) {
            continue;
        }

        string movie_id = tokens[0];
        double avg_rating = stod(tokens[1]);
        int votes = stoi(tokens[2]);

        // Only attach rating if this movie exists in our movie list
        if (rating_by_movie.find(movie_id) != rating_by_movie.end()) {
            rating_by_movie[movie_id].rating = avg_rating;
            rating_by_movie[movie_id].numVotes = votes;
        }
    }

    return rating_by_movie;
}
