#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <sqlite3.h>

const std::string DB_FILE = "scores.db";

const std::string DB_CREATE_SCORES_TABLE = "CREATE TABLE IF NOT EXISTS scores (id INT, difficulty INT, score INT);";

class Database
{
private:
    sqlite3 *DBHandle;
public:
    Database();
    void Open();
    void Close();

    void AddScore(int difficulty, int score);
    int GetHighScore(int difficulty);
};

#endif // DATABASE_H
