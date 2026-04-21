#include "water_seven.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////
char character[FIXED_CHARACTER][MAX_NAME];
int hp[FIXED_CHARACTER];
int skill[FIXED_CHARACTER];
int shipHP, repairCost;
char duel[FIXED_CHARACTER][MAX_NAME];
char resultText[10000];
int grid[MAX_GRID][MAX_GRID];
int rows, cols;
//Task 0
static const string VALID_CHARS[] = {
    "LUFFY", "ZORO", "SANJI", "NAMI", "CHOPPER", "USOPP", "ROBIN"
};

static bool isValidCharacter(const string &name) {
    for (int i = 0; i < FIXED_CHARACTER; i++) {
        if (name == VALID_CHARS[i])
            return true;
    }
    return false;
}
static int clamp(int value, int L, int R) {
    if (value < L) return L;
    if (value > R) return R;
    return value;
}
static bool Is_integer(const string &s) {
    if (s.empty()) return false;

    int i = 0;
    if (s[i] == '-' || s[i] == '+') i++;
    int n = s.size();
    int j = i;

    if (i == n) return false;
    for (; i < n; i++) {
        if (s[i] < '0' || s[i] > '9') return false;
    }
    long long x = 0;
    for (i = j; i < n; i++) {
        x = x * 10 + (s[i] - '0');
        if (x > INT_MAX) return false;
    }
    return true;
}
static int convert(const string &s) {
    int x = 0;
    int i = 0, n = s.size();
    if (s[i] == '-' || s[i] == '+') i++;

    if (i == n) return x;

    for (; i < n; i++) {
        x = x * 10 + (s[i] - '0');
    }
    if (s[0] == '-') x = -x;

    return x;
}
bool readInput(
    const string &filename,
    char character[FIXED_CHARACTER][MAX_NAME], int hp[FIXED_CHARACTER],
    int skill[FIXED_CHARACTER], int &shipHP, int &repairCost) {
    ifstream fin(filename);
    if (!fin.is_open()) return false;

    string line;

    int char_cnt = 0;

    while (getline(fin, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string name, extra;
        int val1, val2;
        if (!(ss >> name)) {
            fin.close();
            return false;
        }
        if (name != "GOING_MERRY" && !isValidCharacter(name)) continue;

        string s1, s2;
        if (!(ss >> s1 >> s2)) {
            if (name == "GOING_MERRY" || isValidCharacter(name)) {
                fin.close();
                return false;
            }
            continue;
        }

        if (!Is_integer(s1) || !Is_integer(s2)) {
            fin.close();
            return false;
        }
        val1 = convert(s1);
        val2 = convert(s2);
        //cout << s1 << " " << s2 << endl;

        if (ss >> extra) {
            fin.close();
            return false;
        }

        if (name == "GOING_MERRY") {
            if (val1 < 0) val1 = 0;
            if (val1 > 1000) val1 = 1000;
            if (val2 < 0) val2 = 0;
            if (val2 > 3000) val2 = 3000;
            shipHP = val1;
            repairCost = val2;
        }
        else if (isValidCharacter(name)) {
            if (val1 < 0) val1 = 0;
            if (val1 > 1000) val1 = 1000;
            if (val2 < 0) val2 = 0;
            if (val2 > 100) val2 = 100;

            int id = -1;

            for (int i = 0; i < char_cnt; ++i) {
                if (strcmp(character[i], name.c_str()) == 0) {
                    id = i;
                    break;
                }
            }

            if (id == -1) {
                if (char_cnt >= FIXED_CHARACTER) {
                    fin.close();
                    return false;
                }
                int len = name.length();

                if (len >= MAX_NAME) len = MAX_NAME - 1;

                for (int i = 0; i < len; i++) {
                    character[char_cnt][i] = name[i];
                }

                character[char_cnt][len] = '\0';

                hp[char_cnt] = val1;
                skill[char_cnt] = val2;
                char_cnt++;
            }
            else {
                hp[id] = val1;
                skill[id] = val2;
            }
        }
    }

    fin.close();
    return true;
}



// Task 1
int damageEvaluation(int shipHP, int repairCost){
    shipHP = clamp(shipHP, 0, 1000);
    repairCost = clamp(repairCost, 0, 3000);
    int x = shipHP, sum = 0;
    while (x > 0) {
        sum += x % 10;
        x /= 10;
    }

    int s = 0;
    for (long i = 1; i * i <= sum; i++) {
        if (sum % i == 0) {
            if (i * i == sum) s += i;
            else s += i + sum / i;
        }
    }

    if (s != 0 && s == 2 * sum && shipHP < 455) {
        repairCost += (repairCost + 1) / 2;
        repairCost = clamp(repairCost, 0, 3000);
        return repairCost;
    }

    //cout << "Result: " << repairCost << endl;
    return repairCost;
}

// Task 2
int conflictSimulation(
    char character[FIXED_CHARACTER][MAX_NAME], int hp[FIXED_CHARACTER], int skill[FIXED_CHARACTER],
    int shipHP, int repairCost) {
        shipHP = clamp(shipHP, 0, 1000);
        repairCost = clamp(repairCost, 0, 3000);
        int conflictIndex = 0;
        int Luffy_skill = 0, Usopp_skill = 0;
        for (int i = 0; i < FIXED_CHARACTER; i++) {
            string s = "";
            for (int j = 0; j < MAX_NAME; j++) {
                if (character[i][j] == '\0') {
                    if (s == "LUFFY") Luffy_skill = skill[i];
                    else if (s == "USOPP") Usopp_skill = skill[i];
                    break;
                }
                s += character[i][j];
            }
        }

        conflictIndex = ceil(Luffy_skill - Usopp_skill + repairCost / 100.0 + (500.0 - shipHP) / 50.0);
        //cout << "ConflictIndex: " << " " << conflictIndex << "\n";
        //cout << "Luffy_skill: " << Luffy_skill << " " << "Usopp_skill: " << Usopp_skill << "\n";

        int id = conflictIndex % 6;

        for (int i = 1; i <= 10; i++) {
            if (id == 0) conflictIndex += 255;
            else if (id == 1) conflictIndex += 20;
            else if (id == 2) conflictIndex += 50;
            else if (id == 3) conflictIndex += 70;
            else if (id == 4) conflictIndex += 90;
            else if (id == 5) conflictIndex += 100;
            id = conflictIndex % 6;
            if (conflictIndex >= 255) break;
        }
        return conflictIndex;
}

// Task 3
int n_support = 0;
void resolveDuel(
    char character[FIXED_CHARACTER][MAX_NAME], int hp[FIXED_CHARACTER], int skill[FIXED_CHARACTER],
    int conflictIndex, int repairCost, char duel[FIXED_CHARACTER][MAX_NAME]){
        //TODO: Output assign to duel parameter
        repairCost = clamp(repairCost, 0, 3000);
        int Luffy_skill = 0, Usopp_skill = 0;
        for (int i = 0; i < FIXED_CHARACTER; i++) {
            string s = "";
            for (int j = 0; j < MAX_NAME; j++) {
                if (character[i][j] == '\0') {
                    if (s == "LUFFY") Luffy_skill = skill[i];
                    else if (s == "USOPP") Usopp_skill = skill[i];
                    break;
                }
                s += character[i][j];
            }
        }

        int U = ceil(Usopp_skill + conflictIndex / 20.0 + repairCost / 500.0);
        int L = Luffy_skill;

        #define getbit(mask, i) ((mask >> i) & 1)
        #define MASK(i) (1LL << i)

        int minz = INT_MAX, minz_cnt = 7;

        int id_mask = 0;
        for (int mask = 0; mask < MASK(FIXED_CHARACTER); mask++) {
            int support = 0, cost = 0, cnt = 0;
            for (int i = 0; i < FIXED_CHARACTER; i++) {
                if (getbit(mask, i)) {
                    string s = "";
                    for (int j = 0; j < MAX_NAME; j++) {
                        if (character[i][j] == '\0') {
                            break;
                        }
                        s += character[i][j];
                    }
                    if (s == "LUFFY" || s == "USOPP") continue;
                    support += skill[i];
                    cost += (hp[i] % 10) + 1;
                    cnt++;
                }
            }

            if (support + L >= U) {
                if (cost < minz) {
                    minz = cost;
                    id_mask = mask;
                    minz_cnt = cnt;
                }
                else if (cost == minz) {
                    if (cnt < minz_cnt) {
                        minz_cnt = cnt;
                        id_mask = mask;
                    }
                }
            }
        }

        //for (int i = 0; i < FIXED_CHARACTER; i++) duel[i][0] = '\0';

        int e = 0;
        for (int i = 0; i < FIXED_CHARACTER; i++) {
            if (getbit(id_mask, i)) {
                for (int j = 0; j < MAX_NAME; j++) {
                    if (character[i][j] == '\0') {
                        duel[e][j] = '\0';
                        break;
                    }
                    duel[e][j] = character[i][j];
                }
                e++;
            }
        }
        n_support = e;
}

// Task 4
static string CP9 = "CP9";
static string ENIESLOBBY = "ENIESLOBBY";
char decode(char x, long k) {
    if (x >= 'a' && x <= 'z') return ((x - 'a' - (k % 26) + 26) % 26 + 'a');
    if (x >= 'A' && x <= 'Z') return ((x - 'A' - (k % 26) + 26) % 26 + 'A');
    if (x >= '0' && x <= '9') return ((x - '0' - (k % 10) + 10) % 10 + '0');
    return x;
}
void decodeCP9Message(char character[FIXED_CHARACTER][MAX_NAME],
    int hp[FIXED_CHARACTER], int skill[FIXED_CHARACTER], int conflictIndex,
    int repairCost, char cipherText[], char resultText[]){
        //TODO: Output assign to resultText parameter
        repairCost = clamp(repairCost, 0, 3000);
        int n = strlen(cipherText);
        int checksum = 0;

        if (n < 3){
            resultText[0] = '\0';
            return;
        }

        checksum = (int)(cipherText[n - 2] - '0') * 10 + (int)(cipherText[n - 1] - '0');


        char message[10000];
        for (int i = 0; i < n - 3; i++){
            message[i] = cipherText[i];
        }
        message[n - 3] = '\0';

        int sum = 0;
        for (int i = 0; message[i] != '\0'; i++){
            sum += (int)message[i];
        }


        if ((sum % 100) != checksum){
            resultText[0] = '\0';
            return;
        }


        int key = (conflictIndex + repairCost) % 26;
        int B = (key % 5) + 4;

        string s[10000];

        for (int i = 0; i <= n / B; i++) s[i] = "";
        int cnt = 0, m = 0;
        for (int i = 0; i < n - 3; i++) {
            cnt++;
            s[m] += cipherText[i];
            if (cnt == B) {
                cnt = 0;
                m++;
            }
        }

        if (cnt) m++;

        string t = "";
        for (int i = 0; i < m; i++) {
            int sz = s[i].size();
            for (int j = 0; j < sz * 1L / 2; j++) {
                char c = s[i][j];
                s[i][j] = s[i][sz - j - 1];
                s[i][sz - j - 1] = c;
            }

            for (int j = 0; j < (int) s[i].size(); j++) {
                if (s[i][j] == ' ') {
                    t += ' ';
                    continue;
                }
                t += decode(s[i][j], key);
            }
        }

        string Text = "";
        for (int i = 0; i < n - 3; i++) {
            bool check = 1;
            if (i >= 2) {
                for (int j = 0; j < (int) CP9.size(); j++) {
                    if (i - 2 + j < (int) t.size() && t[i - 2 + j] != CP9[j]) {
                        check = 0;
                        break;
                    }
                }
            }

            if (check && i >= 2) {
                int e = 0;
                Text = "_TRUE";
                for (; e < (int) t.size(); e++) {
                    resultText[e] = t[e];
                }

                int v = 0;
                for (; v < (int) Text.size(); v++) {
                    resultText[e + v] = Text[v];
                }
                resultText[e + v] = '\0';
                return;
            }

            check = 1;
            if (i >= 9) {
                for (int j = 0; j < (int) ENIESLOBBY.size(); j++) {
                    if (i - 9 + j < (int) t.size() && t[i - 9 + j] != ENIESLOBBY[j]) {
                        check = 0;
                        break;
                    }
                }
            }

            if (check && i >= 9) {
                int e = 0;
                Text = "_TRUE";
                for (; e < (int) t.size(); e++) {
                    resultText[e] = t[e];
                }

                int v = 0;
                for (; v < (int) Text.size(); v++) resultText[e + v] = Text[v];
                resultText[e + v] = '\0';
                return;
            }
        }

        int e = 0;
        Text = "_FALSE";
        for (; e < (int) t.size(); e++) {
            resultText[e] = t[e];
        }

        int v = 0;
        for (; v < (int) Text.size(); v++) resultText[e + v] = Text[v];
        resultText[e + v] = '\0';
        return;
}

// Task 5
//void read_grid() {
//    if (!(cin >> rows >> cols)) return;
//    for (int i = 0; i < rows; i++) {
//        for (int j = 0; j < cols; j++) {
//            cin >> grid[i][j];
//        }
//    }
//}
int analyzeDangerLimit(int grid[MAX_GRID][MAX_GRID], int rows, int cols){

    int maxRowSum = 0, maxCell = -1;
    for (int i = 0; i < rows; i++) {
        int sum = 0;
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] >= 0) sum += grid[i][j];
            if (maxCell < grid[i][j]) maxCell = grid[i][j];
        }
        if (maxRowSum < sum) maxRowSum = sum;
    }

    return maxRowSum + maxCell;
}

long long dp[MAX_GRID][MAX_GRID];
bool evaluateRoute(int grid[MAX_GRID][MAX_GRID], int rows, int cols, int dangerLimit){

    const long long inf = 1000000000LL;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dp[i][j] = inf;
        }
    }

    if (grid[0][0] == -1) return false;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i == 0 && j == 0) {
                dp[i][j] = grid[i][j];
                continue;
            }
            if (grid[i][j] == -1) continue;
            if (i)  {
                if (dp[i][j] > dp[i - 1][j] + grid[i][j] * 1LL) dp[i][j] = dp[i - 1][j] + grid[i][j] * 1LL;
            }
            if (j) {
                if (dp[i][j] > dp[i][j - 1] + grid[i][j] * 1LL) dp[i][j] = dp[i][j - 1] + grid[i][j] * 1LL;
            }
        }
    }
    //cout << "Valid Route: " << dp[rows - 1][cols - 1] << "\n";
    if (dp[rows - 1][cols - 1] < inf && dp[rows - 1][cols - 1] <= dangerLimit) return true;
    return false;
}


////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
