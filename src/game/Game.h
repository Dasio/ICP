/* Authors:
 * Dávid Mikuš      (xmikus15)
 * Peter Hostačný   (xhosta03)
 */



#ifndef GAME_H
#define GAME_H

#include <boost/serialization/string.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <string>
#include <vector>
#include <fstream>

#include "MazeBoard.h"


class Player
{
public:
    std::string name;
    int score;
    Coords position;

    Player() : name{}, score{0}, position{} {}
    Player(const std::string &player_name): name(player_name),score(0) {}

private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &ar, unsigned /*version*/)
    {
        ar & BOOST_SERIALIZATION_NVP(name);
        ar & BOOST_SERIALIZATION_NVP(score);
        ar & BOOST_SERIALIZATION_NVP(position);
    }
};


typedef enum {MOVE, SHIFT} Action;


class Game
{
public:
    MazeBoard labyrinth;

    /**
     * @brief Initialize board, fill with stones, with treasures and randomize players positions
     * here must be minimum of 4 players in the game
     * @param board_size
     * @param treasure_count
     * @return true if was successfully initialized, otherwise false
     */
    bool initialize(int board_size = 7, int treasure_count = 12);

    /**
     * @brief Create new player
     * @param Player name name
     * @return Player number or -1 if there are already 4 players
     */
    int addPlayer(std::string name);

    /**
     * @brief getPlayerName
     * @param Player ID
     * @return Player name
     */
    std::string getPlayerName(uint id);

    /**
     * @brief getPlayerScore
     * @param Player ID
     * @return Player score
     */
    int getPlayerScore(uint id);

    /**
     * @brief getMaxScore
     * @return Score for win
     */
    inline int getMaxScore() { return max_score; }

    /**
     * @brief getPlayersCount
     * @return How many players are playing
     */
    inline int getPlayersCount() { return players.size(); }

    /**
     * @brief Click to the gameboard and take an action if it is allowed
     * @param x
     * @param y
     * @return true if action was allowed, otherwise false
     */
    bool clickBoard(int x, int y);

    /**
     * @brief checkWinner
     * @return nullptr if there is no winner, else pointer to player name
     */
    inline std::string* checkWinner() { return winner; }

    /**
     * @brief actualCard
     * @return int representing the treasure on the top of card_pack, 0 if pack is empty
     */
    int actualCard();

    /**
     * @brief activePlayer
     * @return Player ID
     */
    inline int activePlayer() { return player_on_turn; }

    /**
     * @brief nextAction
     * @return Action which player is going to make
     */
    inline Action nextAction() { return next_action; }

    /**
     * @brief saveGame
     * @param file_name
     * @return
     */
    bool saveGame(std::string file_name);

    /**
     * @brief loadGame
     * @param file_name
     * @return
     */
    bool loadGame(std::string file_name);

    /**
     * @brief undo
     * @return
     */
    bool undo();

private:
    // TODO: vector of DATA CONTAINERS (streams?) with game_state for UNDO

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &ar, unsigned /*version*/)
    {
        ar & BOOST_SERIALIZATION_NVP(card_pack);
        ar & BOOST_SERIALIZATION_NVP(players);

        ar & BOOST_SERIALIZATION_NVP(player_on_turn);
        ar & BOOST_SERIALIZATION_NVP(next_action);
        ar & BOOST_SERIALIZATION_NVP(max_score);
        ar & BOOST_SERIALIZATION_NVP(winner);
    }

    std::vector<int> card_pack;
    std::vector<Player> players;

    int player_on_turn;
    Action next_action;
    int max_score;
    std::string *winner;

    /**
     * @brief streamToState
     * @param stream
     */
    void streamToState(std::fstream &stream);

    /**
     * @brief stateToStream
     * @param stream
     */
    void stateToStream(std::fstream &stream);

    /**
     * @brief try to move player to the coordinates [x,y]
     * @param x
     * @param y
     * @return True if it was successful
     */
    bool tryMovePlayer(int x, int y);
};



#endif // GAME_H
