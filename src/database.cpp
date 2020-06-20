#include "database.hpp"
#include "logger.hpp"

// TODO: parameterize these queries!

int HighScoreCallback(void *output, int count, char **data, char **columns)
{
    char *v = data[0];

    if (v != NULL)
    {
        *(int *)output = (int)std::atoi(v);
    }

    // TODO: this is probably supposed to return something important
    return 0;
}

Database::Database()
{
}

void Database::Open()
{
    if (sqlite3_open_v2(DB_FILE.c_str(), &DBHandle, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK)
    {
        Log("Failed to open SQLite database: " + std::string(sqlite3_errmsg(DBHandle)), LOG_ERROR);
        exit(1);
    }

    // Create table
    if (sqlite3_exec(DBHandle, DB_CREATE_SCORES_TABLE.c_str(), 0, 0, NULL) != SQLITE_OK)
    {
        Log("Failed to create scores table: " + std::string(sqlite3_errmsg(DBHandle)), LOG_ERROR);
    }
}

void Database::Close()
{
}

void Database::AddScore(int level, int score)
{
    std::string DB_INSERT_SCORE = "INSERT INTO scores (difficulty, score) VALUES(" + std::to_string(level) + ", " + std::to_string(score) + ");";
    if (sqlite3_exec(DBHandle, DB_INSERT_SCORE.c_str(), 0, 0, NULL) != SQLITE_OK)
    {
        Log("Failed to insert score: " + std::string(sqlite3_errmsg(DBHandle)), LOG_WARNING);
    }
}

int Database::GetHighScore(int difficulty)
{
    std::string query = "SELECT MAX(score) FROM scores WHERE difficulty = " + std::to_string(difficulty) + ";";

    int highScore = 0;

    sqlite3_exec(DBHandle, query.c_str(), HighScoreCallback, &highScore, NULL);

    return highScore;
}