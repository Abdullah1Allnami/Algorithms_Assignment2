#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unordered_map>
using namespace std;

template <typename TKey, typename TValue>
class SkipList {
private:
    struct Node {
        TKey key;
        TValue value;
        vector<Node*> forward;

        Node(int level, TKey k, TValue v)
            : key(k), value(v), forward(level + 1, nullptr) {}
    };

    int maxLevel;
    float probability;
    int level;
    Node* header;

    int randomLevel() {
        int lvl = 0;
        while ((float)rand() / RAND_MAX < probability && lvl < maxLevel) {
            lvl++;
        }
        return lvl;
    }

public:
    SkipList(int maxLevel, float probability)
        : maxLevel(maxLevel), probability(probability), level(0) {
        header = new Node(maxLevel, TKey{}, TValue{});
    }

    ~SkipList() {
        Node* current = header;
        while (current != nullptr) {
            Node* next = current->forward[0];
            delete current;
            current = next;
        }
    }

    pair<bool, TValue> search(const TKey& key) {
        Node* current = header;

        for (int i = level; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->key < key) {
                current = current->forward[i];
            }
        }

        current = current->forward[0];

        if (current != nullptr && current->key == key) {
            return { true, current->value };
        }
        return { false, TValue{} };
    }

    void insert(const TKey& key, const TValue& value) {
        vector<Node*> update(maxLevel + 1, nullptr);
        Node* current = header;

        for (int i = level; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->key < key) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];

        if (current != nullptr && current->key == key) {
            current->value = value;
            return;
        }

        int newLevel = randomLevel();
        if (newLevel > level) {
            for (int i = level + 1; i <= newLevel; i++) {
                update[i] = header;
            }
            level = newLevel;
        }

        Node* newNode = new Node(newLevel, key, value);
        for (int i = 0; i <= newLevel; i++) {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }
    }

    bool erase(const TKey& key) {
        vector<Node*> update(maxLevel + 1, nullptr);
        Node* current = header;

        for (int i = level; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->key < key) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];

        if (current != nullptr && current->key == key) {
            for (int i = 0; i <= level; i++) {
                if (update[i]->forward[i] != current) break;
                update[i]->forward[i] = current->forward[i];
            }
            delete current;

            while (level > 0 && header->forward[level] == nullptr) {
                level--;
            }
            return true;
        }
        return false;
    }

    vector<pair<TKey, TValue>> getTopN(int n) const {
        vector<pair<TKey, TValue>> result;
        Node* current = header->forward[0];
        while (current != nullptr && n > 0) {
            result.emplace_back(current->key, current->value);
            current = current->forward[0];
            n--;
        }
        return result;
    }
};

class GameManager {
private:
    SkipList<int, string> scoreList;
    unordered_map<string, int> playerScores;  // Track current scores for each player

public:
    GameManager() : scoreList(16, 0.5) {}  // Increased max level for better performance

    void addPlayer(const string& playerId) {
        if (playerScores.find(playerId) == playerScores.end()) {
            playerScores[playerId] = 0;
            scoreList.insert(0, playerId);
        }
    }

    bool removePlayer(const string& playerId) {
        auto it = playerScores.find(playerId);
        if (it != playerScores.end()) {
            scoreList.erase(it->second);
            playerScores.erase(it);
            return true;
        }
        return false;
    }

    bool updateScore(const string& playerId, int newScore) {
        auto it = playerScores.find(playerId);
        if (it != playerScores.end()) {
            int oldScore = it->second;
            scoreList.erase(oldScore);
            scoreList.insert(newScore, playerId);
            it->second = newScore;
            return true;
        }
        return false;
    }

    int getPlayerScore(const string& playerId) const {
        auto it = playerScores.find(playerId);
        return (it != playerScores.end()) ? it->second : -1;
    }

    vector<pair<int, string>> getLeaderboard(int n) const {
        return scoreList.getTopN(n);
    }
};

int main() {
    srand(time(nullptr));

    GameManager game;

    // Test player management
    game.addPlayer("Alice");
    game.addPlayer("Bob");
    game.addPlayer("Charlie");

    // Test score updates
    game.updateScore("Alice", 50);
    game.updateScore("Bob", 40);
    game.updateScore("Charlie", 30);

    // Test score retrieval
    cout << "Alice's score: " << game.getPlayerScore("Alice") << endl;
    cout << "Bob's score: " << game.getPlayerScore("Bob") << endl;

    // Test leaderboard
    cout << "\nLeaderboard:\n";
    for (const auto& player : game.getLeaderboard(3)) {
        cout << player.second << " with score " << player.first << "\n";
    }

    // Test player removal
    cout << "\nRemoving Bob...\n";
    game.removePlayer("Bob");

    cout << "Updated Leaderboard:\n";
    for (const auto& player : game.getLeaderboard(3)) {
        cout << player.second << " with score " << player.first << "\n";
    }

    return 0;
}
