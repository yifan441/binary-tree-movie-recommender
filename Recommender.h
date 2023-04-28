#ifndef RECOMMENDER_INCLUDED
#define RECOMMENDER_INCLUDED

#include <string>
#include <vector>


class UserDatabase;
class MovieDatabase;
class Movie;

struct MovieAndRank
{
    MovieAndRank(const std::string& id, int score)
     : movie_id(id), compatibility_score(score)
    {}

    std::string movie_id;
    int compatibility_score;
};

class Recommender
{
  public:
    Recommender(const UserDatabase& user_database,
                const MovieDatabase& movie_database);
    std::vector<MovieAndRank> recommend_movies(const std::string& user_email,
                                               int movie_count) const; 

  private:
      const UserDatabase& m_userDB;
      const MovieDatabase& m_movieDB;

      struct HelperObj
      {
          HelperObj(int score, float rating, std::string title, std::string id)
              : m_score(score), m_rating(rating), m_title(title), m_id(id)
          {}
          int m_score;
          float m_rating;
          std::string m_title;
          std::string m_id;
      };

      static bool sortHelperObj(const HelperObj& obj1, const HelperObj& obj2);

};

#endif // RECOMMENDER_INCLUDED
