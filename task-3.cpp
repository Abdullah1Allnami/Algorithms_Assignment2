#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
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

    void insert(const TKey& key, const TValue& value) {
        vector<Node*> update(maxLevel + 1, nullptr);
        Node* current = header;

        for (int i = level; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->key > key) {
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

    void erase(const TKey& key) {
        vector<Node*> update(maxLevel + 1, nullptr);
        Node* current = header;

        for (int i = level; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->key > key) {
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
        }
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
    SkipList<int, string> skipList;

public:
    GameManager() : skipList(5, 0.5) {}

    void addPlayer(const string& playerId) {
        skipList.insert(0, playerId);
    }

    void removePlayer(const string& playerId, int score) {
        skipList.erase(score);
    }

    void updateScore(const string& playerId, int oldScore, int newScore) {
        skipList.erase(oldScore);
        skipList.insert(newScore, playerId);
    }

    vector<pair<int, string>> getLeaderboard(int n) const {
        return skipList.getTopN(n);
    }
};

int main() {
    srand(time(nullptr));

    GameManager game;

    // Add players
    game.addPlayer("Alice");
    game.addPlayer("Bob");
    game.addPlayer("Charlie");

    // Update scores
    game.updateScore("Alice", 0, 50);
    game.updateScore("Bob", 0, 40);
    game.updateScore("Charlie", 0, 30);

    // Get leaderboard
    cout << "Leaderboard:\n";
    for (const auto& player : game.getLeaderboard(3)) {
        cout << player.second << " with score " << player.first << "\n";
    }

    return 0;
}
