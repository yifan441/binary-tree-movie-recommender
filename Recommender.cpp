#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "Movie.h"
#include "User.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

Recommender::Recommender(const UserDatabase& user_database,
                         const MovieDatabase& movie_database)
    : m_userDB(user_database), m_movieDB(movie_database)
{
    // Replace this line with correct code.
}

bool Recommender::sortHelperObj(const HelperObj& obj1, const HelperObj& obj2) 
{
    if (obj1.m_score != obj2.m_score)
        return obj1.m_score > obj2.m_score;

    else if (obj1.m_rating != obj2.m_rating)
        return obj1.m_rating > obj2.m_rating;

    else
        return obj1.m_title < obj2.m_title;
}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    User* user = m_userDB.get_user_from_email(user_email);
    if (user == nullptr || movie_count <= 0)
    {
        cout << "Please enter a valid email or movie count!" << endl;
        return vector<MovieAndRank>();
    }

    vector<Movie*> userMovies_v;
    vector<string> user_wh_v = user->get_watch_history(); 

    vector<string> directors_v;
    vector<string> actors_v;
    vector<string> genres_v;

    unordered_map<string, int> scores; // <movieID, score>

    // get list of movies user watched first 
    for (int i = 0; i < user_wh_v.size(); i++)
    {
       Movie* m = m_movieDB.get_movie_from_id(user_wh_v[i]); 

       // need to account for duplicates

       userMovies_v.push_back(m);
    }

    // loop through all the movies the user has watched and give points to all movies that share a
    // director, actor, or genre

    for (int i = 0; i < userMovies_v.size(); i++) // loop through each movie user has watched
    {
        for (int j = 0; j < userMovies_v[i]->get_directors().size(); j++) // loop through each director 
        {
            vector<Movie*> directed_movies = m_movieDB.get_movies_with_director(userMovies_v[i]->get_directors()[j]);

            for (int k = 0; k < directed_movies.size(); k++) // loop through each movie they directed 
            {
                //give points 
                scores[directed_movies[k]->get_id()] += 20; 
            }
        }

        for (int j = 0; j < userMovies_v[i]->get_actors().size(); j++) // loop through each actor
        {
            vector<Movie*> acted_in_movies = m_movieDB.get_movies_with_actor(userMovies_v[i]->get_actors()[j]);

            for (int k = 0; k < acted_in_movies.size(); k++) // loop through each movie they acted in
            {
                scores[acted_in_movies[k]->get_id()] += 30;
            }
        }

        for (int j = 0; j < userMovies_v[i]->get_genres().size(); j++) // loop through each genre
        {
            vector<Movie*> movies_with_this_genre = m_movieDB.get_movies_with_genre(userMovies_v[i]->get_genres()[j]);

            for (int k = 0; k < movies_with_this_genre.size(); k++) // loop through each move with this genre
            {
                scores[movies_with_this_genre[k]->get_id()] += 1;
            }
        }
    }

    // sort the movies by score -> rating -> alphabetical name 

    vector<HelperObj> toSort_vec;

    for (unordered_map<string, int>::iterator it = scores.begin(); it != scores.end(); it++)
    {
        string title = m_movieDB.get_movie_from_id(it->first)->get_title();
        float rating = m_movieDB.get_movie_from_id(it->first)->get_rating();
        HelperObj o(it->second, rating, title, it->first);
        toSort_vec.push_back(o);
    }

    sort(toSort_vec.begin(), toSort_vec.end(), sortHelperObj);

    // make a vector of sorted movies 

    vector<MovieAndRank> sorted;

    for (int i = 0; i < toSort_vec.size() && sorted.size() < movie_count; i++)
    {
        HelperObj& o = toSort_vec[i];

        vector<string>::iterator it = find(user_wh_v.begin(), user_wh_v.end(), o.m_id);  
        
        // if user has already watched this movie or compatibility score is less than 1
        // move onto the next movie
        if (it != user_wh_v.end() || o.m_score < 1)
            continue;

        MovieAndRank mar(o.m_id, o.m_score);
        sorted.push_back(mar);
    }

    // return that vector 

    return sorted;  // Replace this line with correct code.
}


