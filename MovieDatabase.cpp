#include "MovieDatabase.h"
#include "Movie.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <vector>
#include "utility.h"
using namespace std;

MovieDatabase::MovieDatabase()
    : first_time(true)
{
    // Replace this line with correct code.
}

bool MovieDatabase::load(const string& filename)
{
    if (!first_time)
        return false;

    ifstream infile(filename);
    if (!infile)
    {
        cerr << "Error: Cannot open data.txt!" << endl;
        return false;
    }

    int lineNumber = 1;
    string id = "";
    string title = "";
    string release_year = "";
    string director = "";
    string actor = "";
    string genre = "";
    float rating = 0.0f;

    vector<string> directors_vec;
    vector<string> actors_vec;
    vector<string> genres_vec;

    string line;
    while (getline(infile, line))
    {
        if (!line.empty())
        {
            if (lineNumber == 1) { id = line; }
            else if (lineNumber == 2) { title = line; }
            else if (lineNumber == 3) { release_year = line; }
            else if (lineNumber == 4) // directors
            {
                stringstream input(line);
                while (input.good())
                {
                    string single;
                    getline(input, single, ',');
                    directors_vec.push_back(single);
                }
            }
            else if (lineNumber == 5) // actors
            {
                stringstream input(line);
                while (input.good())
                {
                    string single;
                    getline(input, single, ',');
                    actors_vec.push_back(single);
                }
            }
            else if (lineNumber == 6) // genres
            {
                stringstream input(line);
                while (input.good())
                {
                    string single;
                    getline(input, single, ',');
                    genres_vec.push_back(single);
                }
            }
            else { rating = stof(line); } // rating 

            lineNumber++;
        }
        else
        {
            // create Movie object and push it to tree
            Movie entry(id, title, release_year, directors_vec, actors_vec, genres_vec, rating);

            m_id_tree.insert(toLowerCase(id), entry);

            for (int i = 0; i < directors_vec.size(); i++)
                m_director_tree.insert(toLowerCase(directors_vec[i]), entry);

            for (int i = 0; i < actors_vec.size(); i++)
                m_actor_tree.insert(toLowerCase(actors_vec[i]), entry);

            for (int i = 0; i < genres_vec.size(); i++)
                m_genre_tree.insert(toLowerCase(genres_vec[i]), entry);

            // reset variables
            lineNumber = 1;
            directors_vec.clear();
            actors_vec.clear();
            genres_vec.clear();
        }
    }

    // to account for the text file not ending with an empty line
    // this will ensure the last movie got added to the tree
    if (lineNumber != 1)
    {
        Movie entry(id, title, release_year, directors_vec, actors_vec, genres_vec, rating);

        m_id_tree.insert(toLowerCase(id), entry);

        for (int i = 0; i < directors_vec.size(); i++)
            m_director_tree.insert(toLowerCase(directors_vec[i]), entry);

        for (int i = 0; i < actors_vec.size(); i++)
            m_actor_tree.insert(toLowerCase(actors_vec[i]), entry);

        for (int i = 0; i < genres_vec.size(); i++)
            m_genre_tree.insert(toLowerCase(genres_vec[i]), entry);

        // reset variables
        lineNumber = 1;
        directors_vec.clear();
        actors_vec.clear();
        genres_vec.clear();
    }

    first_time = false;
    return true;
}

Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    TreeMultimap<std::string, Movie>::Iterator it = m_id_tree.find(toLowerCase(id));
    if (it.is_valid())
    {
        Movie& found_movie = it.get_value();
        return &found_movie;
    }
    else
    {
        return nullptr;
    }
}

vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    TreeMultimap<std::string, Movie>::Iterator it = m_director_tree.find(toLowerCase(director));
    vector<Movie*> movie_vec;

    while (it.is_valid())
    {
        Movie* entry = new Movie(it.get_value());
        movie_vec.push_back(entry);
        //Movie entry2 = it.get_value();
        //Movie* ptr = &entry2;
        //movie_vec.push_back(ptr);
        it.advance(); 
    }
    return movie_vec;
}

vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    TreeMultimap<std::string, Movie>::Iterator it = m_actor_tree.find(toLowerCase(actor));
    vector<Movie*> actor_vec;

    while (it.is_valid())
    {
        Movie* entry = new Movie(it.get_value());
        actor_vec.push_back(entry);

        //Movie entry2 = it.get_value();
        //Movie* ptr = &entry2;
        //actor_vec.push_back(ptr);
        it.advance();
    }
    return actor_vec;
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    TreeMultimap<std::string, Movie>::Iterator it = m_genre_tree.find(toLowerCase(genre));
    vector<Movie*> genre_vec;

    while (it.is_valid())
    {
        Movie* entry = new Movie(it.get_value());
        genre_vec.push_back(entry);

        //Movie entry2 = it.get_value();
        //Movie* ptr = &entry2;
        //genre_vec.push_back(ptr);
        it.advance();
    }

    return genre_vec;
}
