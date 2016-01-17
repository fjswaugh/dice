#include <iostream>
#include <string>
#include <random>
#include <sstream>
#include <stdexcept>
#include <vector>

class Roll {
public:
    Roll();
    Roll(int number_of_dice, int sides_on_dice);
    std::vector<int> roll() const;
private:
    int m_number_of_dice;
    int m_sides_on_dice;
};

/*
 * Get a random integer between min and max inclusive.
 */
int rand(int min, int max)
{
    static std::random_device rd;
    static std::mt19937_64 mt(rd());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(mt);
}

Roll::Roll()
    : m_number_of_dice{1}, m_sides_on_dice{1}
{}

Roll::Roll(int number_of_dice, int sides_on_dice)
    : m_number_of_dice{number_of_dice}, m_sides_on_dice{sides_on_dice}
{
    if (m_number_of_dice < 0) {
        throw std::runtime_error("bad number of dice");
    }
    if (m_sides_on_dice < 1) {
        throw std::runtime_error("bad number of sides on dice");
    }
}

std::vector<int> Roll::roll() const
{
    std::vector<int> results;
    for (int i = 0; i < m_number_of_dice; ++i) {
        results.push_back(rand(1, m_sides_on_dice));
    }
    return results;
}

std::istream& operator>>(std::istream& is, Roll& r)
{
    int n1, n2;
    char d;
    if (!(is >> d)) {
        r = Roll(1, 20);
        is.clear(std::ios_base::goodbit);
        return is;
    }

    if (d == 'd') {
        if (!(is >> n2)) {
            return is;
        }
        n1 = 1;
    } else {
        is.putback(d);
        if (!(is >> n1 >> d >> n2)) {
            return is;
        }
        if (d != 'd') {
            is.clear(std::ios_base::failbit);
            return is;
        }
    }
    r = Roll(n1, n2);
    return is;
}

std::string format_results(const std::vector<int>& results)
{
    std::ostringstream ss;
    ss << "Individual dice rolls: ";
    const std::size_t max_to_display = 20;
    for (std::size_t i = 0; i < max_to_display && i < results.size(); ++i) {
        ss  << results[i] << ' ';
    }
    if (results.size() > 20) ss << "...";
    int total = std::accumulate(results.begin(), results.end(), 0);
    ss << "\nTotal for roll:        " << total << "\n";
    return ss.str();
}

void prompt()
try {
    std::cout << "> ";

    std::string input;
    if (!std::getline(std::cin, input)) return;

    if (input == "exit" || input == "quit") return;

    std::istringstream ss(input);
    Roll roll;
    if (!(ss >> roll)) {
        throw std::runtime_error("bad format");
    }
    std::cout << format_results(roll.roll());
    prompt();
}
catch (const std::runtime_error& e)
{
    std::cout << "Error: " << e.what() << '\n';
    prompt();
}

int main()
{
    std::cout << "Dice rolling program\n\n";
    prompt();
}

