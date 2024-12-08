#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <unordered_map>

using namespace std;

// Skill that a Pokémon can use
// Each skill has attributes such as name, type, damage, and usage limits.
class Skill {
public:
    string name, type;        // Name and type of the skill (ex. "Thunderbolt", "Electric").
    int damage, maxTry;       // Base damage and maximum usage count for the skill.
    int remainingTry;         // Remaining times the skill can be used.

    // Constructor
    Skill(const string& name, const string& type, int damage, int maxTry)
        : name(name), type(type), damage(damage), maxTry(maxTry), remainingTry(maxTry) {}

    // Use skill. Reduces count by one.
    // Return true if the skill was used successfully, otherwise false.
    bool use() {
        if (remainingTry > 0) {
            --remainingTry; // Decrease usage count
            return true;    // Skill successfully used
        }
        return false; // Skill cannot be used
    }
};

// Pokemon class
class Pokemon {
public:
    string name, type;               // Name and type of the Pokémon (e.g., "Pikachu", "Electric")
    int maxHP, currentHP;            // Maximum and current HP of the Pokémon
    vector<Skill> skills;            // List of skills

    // Constructor
    Pokemon(const string& name, const string& type, int maxHP, const vector<Skill>& skills)
        : name(name), type(type), maxHP(maxHP), currentHP(maxHP), skills(skills) {}

    // Check if the Pokémon is still alive.
    // Returns true if current HP is greater than zero.
    bool isAlive() const {
        return currentHP > 0;
    }

    // Reduces the Pokémon's HP
    // Ensures HP does not drop below zero.
    void takeDamage(int damage) {
        currentHP = max(0, currentHP - damage);
    }
};

// Determines the type effectiveness of an attack against a Pokémon.
// Takes the attacker's type and the defender's type as input.
// Returns an effectiveness value (positive, negative, or neutral).
int getEffectiveness(const string& attackType, const string& defenderType) {
    // Effectiveness map defining interactions between types.
    static const unordered_map<string, unordered_map<string, int>> effectivenessMap = {
        {"Normal", { {"Ground", 0}, {"Electric", 0}, {"Water", 0}, {"Grass", 0}, {"Fire", 0} }},
        {"Ground", { {"Electric", 5}, {"Grass", -3}, {"Fire", 5} }},
        {"Electric", { {"Ground", -3}, {"Water", 5}, {"Grass", -3} }},
        {"Water", { {"Fire", 5}, {"Grass", -3}, {"Ground", 5} }},
        {"Grass", { {"Water", 5}, {"Ground", 5}, {"Fire", -3} }},
        {"Fire", { {"Grass", 5}, {"Water", -3}, {"Ground", -3} }}
    };

    // Retrieve effectiveness value from the map or return 0 if not found.
    if (effectivenessMap.count(attackType) && effectivenessMap.at(attackType).count(defenderType)) {
        return effectivenessMap.at(attackType).at(defenderType);
    }
    return 0; // Default effectiveness value
}

// displayskill info
void displaySkillInfo(int index, const Skill& skill1, const Skill& skill2) {
    cout << "| (" << index << ") " << setw(25) << left << skill1.name
         << "| (" << index << ") " << setw(25) << left << skill2.name << "|\n";
    cout << "|     - Type: " << setw(17) << left << skill1.type
         << "|     - Type: " << setw(17) << left << skill2.type << "|\n";
    cout << "|     - Damage: " << setw(15) << left << skill1.damage
         << "|     - Damage: " << setw(15) << left << skill2.damage << "|\n";
    cout << "|     - Count: " << setw(16) << (to_string(skill1.remainingTry) + "(" + to_string(skill1.maxTry) + ")")
         << "|     - Count: " << setw(16) << (to_string(skill2.remainingTry) + "(" + to_string(skill2.maxTry) + ")") << "|\n";
}

//display battle page
void displayBattlePage(const Pokemon& pokemon1, const Pokemon& pokemon2, const string& latestSkill1, const string& latestSkill2, const string& effectiveness1, const string& effectiveness2, bool isPlayer1Turn) {
    cout << "+-------------------------------------------------------------+\n";
    cout << "| 2024-02 Object-Oriented Programming Pokemon Master          |\n";
    cout << "+------------------------------+------------------------------+\n";
    cout << "| " << setw(29) << left << pokemon1.name + (isPlayer1Turn ? " (*)" : "")
         << "| " << setw(29) << left << pokemon2.name + (!isPlayer1Turn ? " (*)" : "") << "|\n";
    cout << "| Type: " << setw(23) << left << pokemon1.type
         << "| Type: " << setw(23) << left << pokemon2.type << "|\n";
    cout << "| HP: " << setw(25) << left << pokemon1.currentHP
         << "| HP: " << setw(25) << left << pokemon2.currentHP << "|\n";
    cout << "+------------------------------+------------------------------+\n";
    cout << "| Latest Skill: " << setw(15) << left << latestSkill1
         << "| Latest Skill: " << setw(15) << left << latestSkill2 << "|\n";
    cout << "| " << setw(29) << left << effectiveness1
         << "| " << setw(29) << left << effectiveness2 << "|\n";
    cout << "+------------------------------+------------------------------+\n";

    // Displays each skill for both Pokémon.
    for (int i = 0; i < 4; ++i) {
        displaySkillInfo(i, pokemon1.skills[i], pokemon2.skills[i]);
    }
    cout << "+------------------------------+------------------------------+\n";
}

// Allows players to select two Pokémon from a list of available Pokémon.
pair<Pokemon, Pokemon> choosePokemon(const vector<Pokemon>& pokemons) {
    int choice1, choice2;
    cout << "Choose a Pokemon(0~4): ";
    cin >> choice1;
    cout << "Choose a Pokemon(0~4): ";
    cin >> choice2;

    // Ensure the choices are different.
    if (choice1 == choice2) {
        cout << "You have to choose Pokemons different from each other." << endl;
        exit(0); // Terminate if the same Pokémon is chosen.
    }

    return {pokemons[choice1], pokemons[choice2]};
}

// Prompts the player to choose a skill for their Pokémon.
int chooseSkill(const Pokemon& pokemon) {
    int choice;
    do {
        cout << "Choose a skill (0~3): ";
        cin >> choice;
    } while (choice < 0 || choice > 3); // Repeat until a valid choice is made.
    return choice;
}

// Executes a turn in the battle.
void takeTurn(Pokemon& attacker, Pokemon& defender, string& latestSkill, string& effectiveness) {
    int choice = chooseSkill(attacker);
    Skill& skill = attacker.skills[choice];

    // Check if the skill can be used.
    if (!skill.use()) {
        cout << attacker.name << " failed to perform " << skill.name << ".\n\n";
        return;
    }

    // Calculate damage with type effectiveness.
    int damage = skill.damage + getEffectiveness(skill.type, defender.type);
    defender.takeDamage(damage);

    // Update the latest skill and effectiveness feedback.
    latestSkill = skill.name;
    int effectivenessValue = getEffectiveness(skill.type, defender.type);
    effectiveness = (effectivenessValue == 5) ? "It was super effective." : 
                    (effectivenessValue == -3) ? "It was not very effective." : "It was effective.";

    // Display a summary of the turn.
    cout << attacker.name << " used " << skill.name << ".\n";
    cout << effectiveness << "\n\n";
}

// Handles the overall battle logic between two Pokémon.
void battle(Pokemon& pokemon1, Pokemon& pokemon2) {
    string latestSkill1 = "-", latestSkill2 = "-";
    string effectiveness1 = "", effectiveness2 = "";
    bool isPlayer1Turn = true; // Indicates whose turn it is.

    // Continue the battle while both Pokémon are alive.
    while (pokemon1.isAlive() && pokemon2.isAlive()) {
        displayBattlePage(pokemon1, pokemon2, latestSkill1, latestSkill2, effectiveness1, effectiveness2, isPlayer1Turn);

        // Execute the current player's turn.
        if (isPlayer1Turn) {
            takeTurn(pokemon1, pokemon2, latestSkill1, effectiveness1);
        } else {
            takeTurn(pokemon2, pokemon1, latestSkill2, effectiveness2);
        }

        isPlayer1Turn = !isPlayer1Turn; // Switch turns.
    }

    // Display the match result.
    cout << "===============================================================\n";
    if (pokemon1.isAlive()) {
        cout << "Match Result: " << pokemon1.name << " defeats " << pokemon2.name << "\n";
    } else {
        cout << "Match Result: " << pokemon2.name << " defeats " << pokemon1.name << "\n";
    }
}

int main() {
    // Initialize a list of available Pokémon with their respective skills.
    vector<Pokemon> pokemons = {
        {"Pikachu", "Electric", 35, { {"Tackle", "Normal", 4, 5}, {"Grass Knot", "Grass", 8, 5}, {"Thunderbolt", "Electric", 10, 5}, {"Megabolt", "Electric", 15, 3} }},
        {"Dratini", "Water", 41, { {"Wrap", "Normal", 4, 10}, {"Aqua Tail", "Water", 3, 5}, {"Water Pulse", "Water", 13, 2}, {"Hyper Beam", "Normal", 20, 1} }},
        {"Eevee", "Normal", 55, { {"Tackle", "Normal", 4, 5}, {"Sand Attack", "Ground", 8, 3}, {"Bite", "Normal", 12, 3}, {"Rain Dance", "Water", 15, 1} }},
        {"Charmander", "Fire", 39, { {"Tackle", "Normal", 4, 5}, {"Flamethrower", "Fire", 11, 5}, {"Dig", "Ground", 7, 5}, {"Heat Wave", "Fire", 14, 5} }},
        {"Palkia", "Water", 90, { {"Hydro Pump", "Water", 12, 10}, {"Earth Power", "Ground", 15, 10}, {"Surf", "Water", 13, 10}, {"Spatial Rend", "Normal", 30, 10} }}
    };

    // Allow players to choose their Pokémon.
    auto chosenPokemons = choosePokemon(pokemons);
    Pokemon& pokemon1 = chosenPokemons.first;
    Pokemon& pokemon2 = chosenPokemons.second;

    // Start the battle between the two chosen Pokémon.
    battle(pokemon1, pokemon2);
    return 0; // End of program.
}
