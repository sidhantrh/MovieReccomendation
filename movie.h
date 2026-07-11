// Created by sidha on 7/2/2026.
//
#include <filesystem>
#include <vector>
#include <iostream>
#include <cmath>
#include <unordered_map>
#include <algorithm>
#include "mheap.h"
#include "RBT.h"
#include <chrono>


using namespace std;
#ifndef PROJECT2_MOVIE_H
#define PROJECT2_MOVIE_H



struct movie {
    string id;
    string title;

    vector<string> genres;
    int year;
    int runtime;

    vector<string> directors;
    vector<string> cast;

    string country;

    double rating;
    int numVotes;
    int recommendationScore = 0;
};

vector<movie> loadMovies(const string& filename);
vector<string> split(const string& str, char delimiter);
unordered_map<string,movie> loadratings(const string& filename, vector<movie> movies);
unordered_map<string,string> personNames(const string& filename);
void loadregions(const string& filename, unordered_map<string, movie>& movieMap);
void loadPrincipals( const string& filename, unordered_map<string, movie>& movieMap,const unordered_map<string, string>& people);
void calculateRecommendations(const movie& targetMovie, unordered_map<string, movie>& movieMap);

#endif //PROJECT2_MOVIE_H
