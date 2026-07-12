//
// Created by sidha on 7/2/2026.
#include "movie.h"
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>

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
    int count = 0;

    // read each line
    while (getline(file, line)) {
        count++;

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

// ratings by movie map generation
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

// Person Names by id map generation
unordered_map<string,string> personNames(const string& filename) {
    unordered_map<string,string> person_names_by_id;
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: could not open file " << filename << endl;
        return person_names_by_id;
    }
    int count = 0;



    string line;

    // Skip header line
    getline(file, line);

    // read each line
    while (getline(file, line)) {

        count++;
        
        vector<string> tokens = split(line, '\t');

        if (tokens.size() < 2) {
            continue;
        }



        // Skip missing names/ID
        if (tokens[0] == "\\N" || tokens[1] == "\\N") {
            continue;
        }


        person_names_by_id.emplace(tokens[0],tokens[1]);
    }

    return person_names_by_id;
}

void loadPrincipals(
    const string& filename,
    unordered_map<string, movie>& movieMap,
    const unordered_map<string, string>& people
) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error: could not open file "
             << filename << endl;
        return;
    }

    string line;
    getline(file, line);

    int count = 0;

    while (getline(file, line)) {

        count++;

        vector<string> tokens = split(line, '\t');


        if (tokens.size() < 4) {
            continue;
        }

        string movie_id = tokens[0];
        string person_id = tokens[2];
        string category = tokens[3];

        auto movie_it = movieMap.find(movie_id);

        if (movie_it == movieMap.end()) {
            continue;
        }


        auto person_it = people.find(person_id);

        if (person_it == people.end()) {
            continue;
        }


        string person_name = person_it->second;

        if (category == "actor" || category == "actress") {
            movie_it->second.cast.push_back(person_name);
        }

        if (category == "director") {
            movie_it->second.directors.push_back(person_name);
        }
    }
    file.close();
}

void loadregions(const string& filename,unordered_map<string, movie>& movieMap) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error: could not open file " << filename << endl;
        return;
    }

    string line;
    getline(file, line);

    int count = 0;

    while (getline(file, line)) {
        count++;

        vector<string> tokens = split(line, '\t');

        if (tokens.size() < 4) {
            continue;
        }

        if (tokens[0] == "\\N" || tokens[3] == "\\N") {
            continue;
        }

        auto movie_it = movieMap.find(tokens[0]);

        if (movie_it != movieMap.end()) {
            movie_it->second.country = tokens[3];
        }

    }

}

void calculateRecommendations(const movie& targetMovie, unordered_map<string, movie>& movieMap) {
    for (auto& pair : movieMap) {
        movie& currentMovie = pair.second;

        // Reset score 
        currentMovie.recommendationScore = 0;

        // Skip the target movie so we don't recommend the exact same movie to the user
        if (currentMovie.id == targetMovie.id) {
            continue;
        }

        int score = 0;

        // 1. Director (+15)
        bool directorMatch = false;
        for (const string& d1 : targetMovie.directors) {
            for (const string& d2 : currentMovie.directors) {
                if (d1 == d2 && !d1.empty()) {
                    score += 15;
                    directorMatch = true;
                    break;
                }
            }
            if (directorMatch) break;
        }

        // 2. Genre (+10 per shared genre)
        int genreMatches = 0;
        for (const string& g1 : targetMovie.genres) {
            for (const string& g2 : currentMovie.genres) {
                if (g1 == g2 && !g1.empty()) {
                    genreMatches++;
                }
            }
        }
        score += min(genreMatches * 10, 30);

        // 3. Cast (+5 per shared actor, capped at +20)
        int castMatches = 0;
        for (const string& a1 : targetMovie.cast) {
            for (const string& a2 : currentMovie.cast) {
                if (a1 == a2 && !a1.empty()) {
                    castMatches++;
                }
            }
        }
        score += min(castMatches * 5, 20);

        // 4. Release Year (+8 within 3 years, +6 within 5, +3 within 10)
        if (targetMovie.year > 0 && currentMovie.year > 0) {
            int yearDiff = abs(targetMovie.year - currentMovie.year);
            if (yearDiff <= 3) {
                score += 8;
            } else if (yearDiff <= 5) {
                score += 6;
            } else if (yearDiff <= 10) {
                score += 3;
            }
        }

        // 5. Runtime (+3 within 30 minutes, +1 within 45 minutes)
        if (targetMovie.runtime > 0 && currentMovie.runtime > 0) {
            int runtimeDiff = abs(targetMovie.runtime - currentMovie.runtime);
            if (runtimeDiff <= 30) {
                score += 3;
            } else if (runtimeDiff <= 45) {
                score += 1;
            }
        }

        // 6. Country (+4)
        if (targetMovie.country == currentMovie.country && !targetMovie.country.empty()) {
            score += 4;
        }

        // Save the score 
        currentMovie.recommendationScore = score;
    }
}












