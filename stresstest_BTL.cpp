/*
 * ============================================================
 * STRESS TESTER — compares two executables on random inputs
 *
 * Build:
 * g++ -O2 -o stress stress_test.cpp
 *
 * Usage (Windows):
 * stresstest_BTL.exe water_copy.exe wcopy.exe [num_tests] [seed]
 *
 * Usage (Linux/Mac):
 * ./stress ./sol_a ./sol_b [num_tests] [seed]
 * ============================================================
 */
#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cassert>

using namespace std;

// ─── random helpers ──────────────────────────────────────────
static unsigned rng_state;
#define rd rand
unsigned lcg() {
    rng_state = rng_state * 1664525u + 1013904223u;
    return rng_state;
}

int rng(int lo, int hi) {   // inclusive [lo, hi]
    return lo + (int)(rd() * rd()  % (unsigned)(hi - lo + 1));
}

// ─── generate opw_tc_01_input.txt ────────────────────────────
// Fixed crew: always ZORO LUFFY USOPP CHOPPER NAMI SANJI ROBIN
// Only hp, skill, shipHP, repairCost and line order are randomised.
// GOING_MERRY is inserted at a random position among the 7 crew lines.
string generateInputFile(const string& path) {
    vector<string> validCrew = {
        "ZORO", "LUFFY", "USOPP", "CHOPPER", "NAMI", "SANJI", "ROBIN"
    };

    vector<string> badNames = {
        "luffy", "Z0RO", "N@MI", "RO_BIN", "CHOPPER!", "US-OPP",
        "123", "???", "MERRY", "LUFFY1", "_NAMI_", "SANJII", "none"
    };

    vector<string> badNums = {
        "abc", "1.5", "--3", "+", "-", "NaN", "inf", "12x",
        "999999999999999999999", "?"
    };

    auto pick = [&](const vector<string>& v) {
        return v[rng(0, (int)v.size() - 1)];
    };

    auto goodInt = [&](int l, int r) {
        return to_string(rng(l, r));
    };

    auto badToken = [&]() {
        return pick(badNums);
    };

    vector<string> lines;
    vector<string> usedNames;

    // ===== 1. ÉP ĐỦ VALID CREW =====
    for (auto &name : validCrew) {
        string hp = goodInt(0, 1000);
        string skill = goodInt(0, 100);
        lines.push_back(name + " " + hp + " " + skill);
        usedNames.push_back(name);
    }

    // ===== 2. SINH THÊM TEST BẨN =====
    int extraLines = rng(5, 12);

    for (int i = 0; i < extraLines; ++i) {
        int kind = rng(0, 6);

        string name, hp, skill, extra;

        switch (kind) {
        case 0: // duplicate hợp lệ
            name = pick(validCrew);
            hp = goodInt(0, 1000);
            skill = goodInt(0, 100);
            lines.push_back(name + " " + hp + " " + skill);
            break;

        case 1: // tên sai
            name = pick(badNames);
            hp = goodInt(0, 1000);
            skill = goodInt(0, 100);
            lines.push_back(name + " " + hp + " " + skill);
            break;

        case 2: // hp sai
            name = pick(validCrew);
            hp = badToken();
            skill = goodInt(0, 100);
            lines.push_back(name + " " + hp + " " + skill);
            break;

        case 3: // skill sai
            name = pick(validCrew);
            hp = goodInt(0, 1000);
            skill = badToken();
            lines.push_back(name + " " + hp + " " + skill);
            break;

        case 4: // thiếu field
            name = (rng(0,1) ? pick(validCrew) : pick(badNames));
            if (rng(0,1)) {
                lines.push_back(name);
            } else {
                lines.push_back(name + " " + goodInt(0,1000));
            }
            break;

        case 5: // thừa field
            name = (rng(0,1) ? pick(validCrew) : pick(badNames));
            hp = (rng(0,1) ? goodInt(0,1000) : badToken());
            skill = (rng(0,1) ? goodInt(0,100) : badToken());
            extra = pick(badNames);
            lines.push_back(name + " " + hp + " " + skill + " " + extra);
            break;

        case 6: // duplicate tên cũ (random từ usedNames)
            name = usedNames[rng(0, (int)usedNames.size() - 1)];
            hp = goodInt(0, 1000);
            skill = goodInt(0, 100);
            lines.push_back(name + " " + hp + " " + skill);
            break;
        }
    }

    // ===== 3. SHUFFLE =====
    for (int i = (int)lines.size() - 1; i > 0; --i) {
        int j = rng(0, i);
        swap(lines[i], lines[j]);
    }

    // ===== 4. GOING_MERRY =====
    int gmPos = rng(0, (int)lines.size());
    int shipHP = rng(0, 1000);
    int repairCost = rng(0, 3000);

    ofstream fout(path);
    assert(fout.is_open());

    for (int i = 0; i <= (int)lines.size(); ++i) {
        if (i == gmPos) {
            fout << "GOING_MERRY " << shipHP << " " << repairCost << "\n";
        }
        if (i < (int)lines.size()) {
            fout << lines[i] << "\n";
        }
    }

    fout.close();

    ostringstream oss;
    oss << "ship=" << shipHP
        << " repair=" << repairCost
        << " totalLines=" << lines.size();

    return oss.str();
}

// ─── generate cipherText for Task 4 ──────────────────────────
string generateCipherText() {
    int len = rng(10, 50); // Độ dài mật thư ngẫu nhiên
    string s = "";
    long sum = 0;

    // Khởi tạo phần ký tự (chỉ bao gồm a-z, A-Z, 0-9)
    for (int i = 0; i < len; ++i) {
        int type = rng(0, 1);
        char c;
        if (type == 0) c = (char)rng('a', 'z');
        else if (type == 1) c = (char)rng('A', 'Z');

        s += c;
        sum = (sum + (long)c) % 100;
    }

    // 80% đúng checksum (hợp lệ), 20% sai checksum (test nhánh false)
    long XY = sum;
    if (rng(1, 100) > 80) {
        XY = (sum + rng(1, 99)) % 100;
    }

    // Gắn #XY vào cuối chuỗi
    string t;
    if (XY < 10) t = '0' + to_string(XY);
    else t = to_string(XY);
    s += "#" + t;

    // Thỉnh thoảng cài cắm chữ CP9 hoặc ENIESLOBBY vào test case
    // để tăng tỉ lệ ra resultText là _TRUE
    if (rng(1, 100) > 70) {
        if (rng(0, 1)) s.insert(rng(0, s.length() - 5), "CP9");
        else s.insert(rng(0, s.length() - 5), "ENIESLOBBY");
    }

    return s;
}

// ─── generate stdin for Task 4 & 5 ───────────────────────────
string generateStdin() {
    ostringstream oss;

    // --- Ghi cipherText cho Task 4 trước ---
    oss << generateCipherText() << "\n";

    // --- Ghi Grid cho Task 5 ---
    int rows = rng(1, 20);
    int cols = rng(1, 20);
    oss << rows << " " << cols << "\n";
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // -1 (obstacle) with ~10% probability
            int v = (rng(1, 10) == 1) ? -1 : rng(0, 3);
            oss << v;
            if (j + 1 < cols) oss << " ";
        }
        oss << "\n";
    }
    return oss.str();
}

// ─── run one executable ──────────────────────────────────────
string runExe(const string& exe, const string& stdinData) {
#ifdef _WIN32
    string tmpIn  = "_stress_in.txt";
    string tmpOut = "_stress_out.txt";
    string devNull = " 2>nul";
#else
    string tmpIn  = "/tmp/_stress_in.txt";
    string tmpOut = "/tmp/_stress_out.txt";
    string devNull = " 2>/dev/null";
#endif

    {
        ofstream f(tmpIn);
        f << stdinData;
    }

    string cmd = exe + " < " + tmpIn + " > " + tmpOut + devNull;
    int ret = system(cmd.c_str());
    (void)ret;

    ifstream f(tmpOut);
    ostringstream oss;
    oss << f.rdbuf();
    return oss.str();
}

// ─── compare outputs line by line ────────────────────────────
bool compareOutputs(const string& a, const string& b, string& firstDiff) {
    istringstream sa(a), sb(b);
    string la, lb;
    int line = 0;
    while (true) {
        bool ga = (bool)getline(sa, la);
        bool gb = (bool)getline(sb, lb);
        ++line;
        if (!ga && !gb) return true;
        if (ga != gb) {
            ostringstream oss;
            oss << "Line " << line << ": one stream ended early.\n";
            oss << "  exe1: " << (ga ? la : "<EOF>") << "\n";
            oss << "  exe2: " << (gb ? lb : "<EOF>") << "\n";
            firstDiff = oss.str();
            return false;
        }
        if (la != lb) {
            ostringstream oss;
            oss << "Line " << line << " differs:\n";
            oss << "  exe1: " << la << "\n";
            oss << "  exe2: " << lb << "\n";
            firstDiff = oss.str();
            return false;
        }
    }
}

// ─── main ────────────────────────────────────────────────────
int main(int argc, char* argv[]) {
    srand(time(NULL));
    if (argc < 3) {
        fprintf(stderr,
            "Usage: %s <exe1> <exe2> [num_tests=100] [seed=time]\n",
            argv[0]);
        return 1;
    }

    string   exe1 = argv[1];
    string   exe2 = argv[2];
    int      T    = (argc >= 4) ? atoi(argv[3]) : 100;
    unsigned seed = (argc >= 5) ? (unsigned)atoi(argv[4]) : (unsigned)time(nullptr);

    rng_state = seed;
    printf("Stress test: %s  vs  %s\n", exe1.c_str(), exe2.c_str());
    printf("Tests: %d   Seed: %u\n\n", T, seed);

    const string INPUT_FILE = "opw_tc_01_input.txt";

    int passed = 0, failed = 0;

    for (int t = 1; t <= T; ++t) {
        string summary   = generateInputFile(INPUT_FILE);
        string stdinData = generateStdin();

        string out1 = runExe(exe1, stdinData);
        string out2 = runExe(exe2, stdinData);

        string diff;
        if (compareOutputs(out1, out2, diff)) {
            ++passed;
            printf("\r  [%4d/%4d]  PASS", t, T);
            fflush(stdout);
        } else {
            ++failed;
            printf("\n");
            printf("\033[31m==== MISMATCH FOUND ====\033[0m\n");
            printf("Test #%d   (%s)\n\n", t, summary.c_str());

            printf("----- opw_tc_01_input.txt ------------\n");
            {
                ifstream f(INPUT_FILE);
                string line;
                while (getline(f, line)) printf("  %s\n", line.c_str());
            }

            printf("\n--- stdin (Task 4 & 5) -----------\n");
            {
                istringstream ss(stdinData);
                string line;
                while (getline(ss, line)) printf("  %s\n", line.c_str());
            }

            printf("\n--- First diverging line ------------------\n%s\n", diff.c_str());

            printf("--- %s output -----------------------\n", exe1.c_str());
            {
                istringstream ss(out1);
                string line;
                while (getline(ss, line)) printf("  %s\n", line.c_str());
            }

            printf("\n--- %s output -----------------------\n", exe2.c_str());
            {
                istringstream ss(out2);
                string line;
                while (getline(ss, line)) printf("  %s\n", line.c_str());
            }

            printf("\nSeed to reproduce: %u\n", seed);
            printf("Re-run:  %s %s %s 1 %u\n\n",
                   argv[0], exe1.c_str(), exe2.c_str(), seed);
            printf("Input file preserved at: %s\n", INPUT_FILE.c_str());
            break;
        }
        //cout << "OK!" << "\n";
    }

    printf("\n\n");
    if (failed == 0)
        printf("\033[32mAll tests passed!\033[0m\n");
    else
        printf("\033[31mTest FAILED --- see output above.\033[0m\n");

    printf("Passed: %d / %d\n", passed, passed + failed);
    return (failed == 0) ? 0 : 1;
}
