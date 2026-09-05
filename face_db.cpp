#include "face_db.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;
using namespace cv;

vector<KnownFace> loadDatabase(const string& path){
    vector<KnownFace> db;
    ifstream file(path);
    if (!file.is_open())
        return db;

    string line;
    int lineNumber = 0;
    while (getline(file, line)){
        lineNumber++;
        if (line.empty()) continue;

        stringstream ss(line);
        string token;
        getline(ss, token, ',');

        KnownFace kf;
        kf.name = token;

        vector<float> values;
        bool malformed = false;
        while (getline(ss, token, ',')){
            try{
                values.push_back(stof(token));
            }
            catch (const exception&){
                malformed = true;
                break;
            }
        }

        if (malformed || values.empty()){
            cout << "Warning: skipping malformed row " << lineNumber
                 << " in " << path << endl;
            continue;
        }

        kf.embedding = Mat(1, (int)values.size(), CV_32F);
        for (size_t i = 0; i < values.size(); i++)
            kf.embedding.at<float>(0, (int)i) = values[i];

        db.push_back(kf);
    }
    return db;
}

void appendToDatabase(const string& path, const KnownFace& kf){
    ofstream file(path, ios::app);
    file << kf.name;
    for (int i = 0; i < kf.embedding.cols; i++)
        file << "," << kf.embedding.at<float>(0, i);
    file << "\n";
}

int deleteFromDatabase(const string& path, const string& name){
    vector<KnownFace> db = loadDatabase(path);
    size_t before = db.size();

    db.erase(remove_if(db.begin(), db.end(),
                        [&](const KnownFace& kf) { return kf.name == name; }),
              db.end());

    size_t removed = before - db.size();
    if (removed == 0)
        return 0;

    ofstream file(path, ios::trunc);
    for (const KnownFace& kf : db){
        file << kf.name;
        for (int i = 0; i < kf.embedding.cols; i++)
            file << "," << kf.embedding.at<float>(0, i);
        file << "\n";
    }
    return (int)removed;
}

void listDatabase(const string& path){
    vector<KnownFace> db = loadDatabase(path);
    if (db.empty()){
        cout << "Database is empty (" << path << ")." << endl;
        return;
    }

    vector<pair<string, int>> counts;
    for (const KnownFace& kf : db){
        auto it = find_if(counts.begin(), counts.end(),
                           [&](const pair<string, int>& p) { return p.first == kf.name; });
        if (it == counts.end())
            counts.push_back({ kf.name, 1 });
        else
            it->second++;
    }

    cout << counts.size() << " enrolled name(s), " << db.size() << " total sample(s):" << endl;
    for (const auto& p : counts)
        cout << "  - " << p.first << " (" << p.second << " sample(s))" << endl;
}