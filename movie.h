//
// Created by sidha on 7/2/2026.
//
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;
#ifndef PROJECT2_MOVIE_H
#define PROJECT2_MOVIE_H



struct movie {
    string id;
    string title;
    vector<string> genres;
    int year;
    int runtime;
    string director;
    vector<string> cast;
    string language;
    string studio;
    string country;
    string ageRating;
    double imdbRating;
    double rating;
    int numVotes;
    int recommendationScore = 0;
};

vector<movie> loadMovies(const string& filename);
vector<string> split(const string& str, char delimiter);
unordered_map<string,movie> loadratings(const string& filename, vector<movie> movies);
vector<movie> getMovieList(const unordered_map<string, movie>& movies);
#endif //PROJECT2_MOVIE_H
