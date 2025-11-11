#include "TUI.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <vector>
#include <ctime>
#include <limits>
#include <string>
#define NOMINMAX
#include <windows.h>
#include <cstdlib>
#include <conio.h>

//using namespace std;
using scheduler::Task;
using namespace std;


namespace scheduler {

    // ---- helpers ----
    void hideCursor() {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO ci;
        GetConsoleCursorInfo(h, &ci);
        ci.bVisible = FALSE;
        SetConsoleCursorInfo(h, &ci);
    }
    void showCursor() {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO ci;
        GetConsoleCursorInfo(h, &ci);
        ci.bVisible = TRUE;
        SetConsoleCursorInfo(h, &ci);
    }


    typedef long long ll;
    static int ymd(int y, int m, int d) { return y * 10000 + m * 100 + d; }

    static void splitTime(long long ymdhm, int& y, int& m, int& d, int& hh, int& mm) {
        if (ymdhm < 0) { y = m = d = hh = mm = -1; return; }
        y = int(ymdhm / 100000000); ymdhm %= 100000000;
        m = int(ymdhm / 1000000);   ymdhm %= 1000000;
        d = int(ymdhm / 10000);     ymdhm %= 10000;
        hh = int(ymdhm / 100);      mm = int(ymdhm % 100);
    }



    static string hhmm(long long ymdhm) {
        int y, m, d, h, mm; splitTime(ymdhm, y, m, d, h, mm);
        if (h < 0) return "시간 미정";
        ostringstream os; os << setw(2) << setfill('0') << h << ":" << setw(2) << mm;
        return os.str();
    }

    static int weekdayOf(int y, int m, int d) { // 1=Mon..7=Sun
        std::tm tm{}; tm.tm_year = y - 1900; tm.tm_mon = m - 1; tm.tm_mday = d;
        std::mktime(&tm); int w = tm.tm_wday; if (w == 0) w = 7; return w;
    }

    static int lastDayOfMonth(int y, int m) {
        static int md[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
        if (m != 2) return md[m];
        if ((y % 400 == 0) || (y % 4 == 0 && y % 100 != 0)) return 29; return 28;
    }

    static void printTaskLine(Task* t) {   //gettype 수정하기
        long long s, e; t->getTime(s, e);
        string due;
        int y, m, d, hh, mm;
        splitTime(t->getEndDate(), y, m, d, hh, mm);
        due += to_string(y); due += "년 ";
        due += to_string(m); due += "월 ";
        due += to_string(d); due += "일 ";
        due += to_string(hh); due += "시 ";
        due += to_string(mm); due += "분 ";
        cout << "  [" << t->getTaskNum() << "] "
            << t->getTaskName() << " "
            << (t->isfinished() ? "(완료) " : "")
            << hhmm(s) << " - " << hhmm(e)
            << "  | type=" << t->getType()
            << "  | due=" << due << "\n";
    }

    void TUI::get_current_time(int& year, int& month, int& day) {
        std::time_t now_utc = std::time(nullptr);
        std::time_t kst_epoch = now_utc + 9 * 60 * 60; // UTC+9

        // gmtime은 static tm*를 반환하므로 즉시 복사해서 사용
        std::tm tm = *std::gmtime(&kst_epoch);

        year = tm.tm_year + 1900;
        month = tm.tm_mon + 1;   // 1..12
        day = tm.tm_mday;      // 1..31
    }

    void TUI::get_next_day(int& year, int& month, int& day, int offset) {

        struct tm timeinfo = { 0 };
        timeinfo.tm_year = year - 1900; // 1900년 기준
        timeinfo.tm_mon = month - 1;    // 0~11
        timeinfo.tm_mday = day;

        // 필요하면 시간, 분, 초도 설정 가능
        timeinfo.tm_hour = 0;
        timeinfo.tm_min = 0;
        timeinfo.tm_sec = 0;

        // mktime을 이용해 time_t로 변환
        time_t t = mktime(&timeinfo) + 9*60*60;
        t += offset * (24 * 60 * 60);

        timeinfo = *std::gmtime(&t);
        year = timeinfo.tm_year;
        month = timeinfo.tm_mon + 1;   // 1..12
        day = timeinfo.tm_mday;      // 1..31
    }

    void TUI::get_next_week(int& year, int& month, int& day, int offset) {
        struct tm timeinfo = { 0 };
        timeinfo.tm_year = year - 1900; // 1900년 기준
        timeinfo.tm_mon = month - 1;    // 0~11
        timeinfo.tm_mday = day;

        // 필요하면 시간, 분, 초도 설정 가능
        timeinfo.tm_hour = 0;
        timeinfo.tm_min = 0;
        timeinfo.tm_sec = 0;

        // mktime을 이용해 time_t로 변환
        time_t t = mktime(&timeinfo) + 9 * 60 * 60;
        t += offset * (7*24 * 60 * 60);

        timeinfo = *std::gmtime(&t);
        year = timeinfo.tm_year;
        month = timeinfo.tm_mon + 1;   // 1..12
        day = timeinfo.tm_mday;      // 1..31
    }

    void TUI::get_next_month(int& year, int& month, int& day, int offset) {
       
        month += offset;
        if (month == 0) {
            month = 12;
            year -= 1;
        }
        else if(month==13){
            month = 1;
            year += 1;
        }
    }

    // ---- TUI methods ----

    void TUI::mainMenu() {
        hideCursor();

        int year = 0, month = 0, day = 0;
        get_current_time(year, month, day);
        showMonth(year, month);
        int last_choice = 1;

        if (!c) c = new Calender();
        while (true) {

            int offset = 0;
            if (last_choice < 4 && last_choice>0) {
                int t = _getch();
                switch (t) {
                case 224: case 0: {
                    t = _getch();
                    switch (t) {
                    case 75: offset = -1; break;
                    case 77: offset = 1; break;
                    }
                } break;
                default: last_choice = 0; break;
                }
                
                switch (last_choice) {
                case 1: system("cls"); get_next_month(year, month, day, offset); showMonth(year, month); break;
                case 2: system("cls"); get_next_week(year, month, day, offset); showWeek(year, month, day);  break;
                case 3: system("cls"); get_next_day(year, month, day, offset); showDay(year, month, day);   break;
                }
                continue;
            }
            
            system("cls");

            cout << "\n===== SCHEDULER (TUI) =====\n"
                << "1) 월 보기   2) 주 보기   3) 일 보기\n"
                << "4) 작업 추가 5) 작업 수정 6) 작업 삭제\n"
                << "7) 통계      8) 간격변경   0) 종료\n";


            int cmd; if (!(cmd = (_getch()-'0'))) return;
            if (cmd == 0) break;


            get_current_time(year, month, day);
            system("cls");
            switch (cmd) {
            case 1: showMonth(year, month); last_choice = 1; break;
            case 2: showWeek(year, month, day); last_choice = 2; break;
            case 3: showDay(year, month, day); last_choice = 3;  break;
            case 4: showCursor(); addTask(); last_choice = 4; hideCursor();  break;
            case 5: showCursor(); editTask(); last_choice = 5; hideCursor(); break;
            case 6: showCursor(); deleteTask(); last_choice = 6; hideCursor(); break;
            case 7: showStatistics(); last_choice = 7; break;
            case 8: showCursor(); changeInterval(); last_choice = 8; hideCursor(); break;
            default: last_choice = 0; cout << "메뉴를 다시 선택하세요.\n";
            }
        }
    }


    // 월 뷰: YYYY MM 입력 → 월 달력 + 일자별 작업수
    void TUI::showMonth(int year, int month) {
        int y=year, m=month;
        /*cout << "연도 월 입력 (예: 2025 10): ";
        cin >> y >> m;*/
        vector<Task*> tasks;
        int dummyDay = 1;
        if (!c->get_Month(tasks, y, m, dummyDay)) {
            cout << "해당 월 데이터가 없습니다.\n"; //return;
        }
        // 일자별 개수 집계
        map<int, int> cnt;
        for (Task* t : tasks) {
            long long s, e; t->getTime(s, e);
            int yy, MM, dd, hh, mm; splitTime(s, yy, MM, dd, hh, mm);
            int date = (s >= 0) ? (yy * 10000 + MM * 100 + dd) : t->getEndDate();
            int d = date % 100; cnt[d]++;
        }

        int firstW = weekdayOf(y, m, 1); // 1..7
        int lastD = lastDayOfMonth(y, m);

        cout << "\n=== " << y << "." << m << " ===\n";
        cout << " Mo Tu We Th Fr Sa Su\n";
        int col = 1;
        for (int i = 1; i < firstW; i++) { cout << "   "; col++; }
        for (int d = 1; d <= lastD; ++d) {
            cout << setw(2) << d;
            if (cnt[d]) cout << "*"; else cout << " ";
            if (col % 7 == 0) cout << "\n"; else cout << " ";
            col++;
        }
        cout << "\n* 표시는 해당 일에 작업이 존재함을 의미\n";
    }

    // 주 뷰: 기준 YYYY MM DD 입력 → 그 주(월~일) 각 일자와 작업 목록
    void TUI::showWeek(int year, int month, int day) {
        int y=year, m=month, d=day;
        /*cout << "연도 월 일 입력 (예: 2025 10 28): ";
        cin >> y >> m >> d;*/
        vector<Task*> tasks;
        c->get_Week(tasks, y, m, d);
        // 날짜별 그룹핑
        map<int, vector<Task*> > byDate;
        for (Task* t : tasks) {
            long long s, e; t->getTime(s, e);
            int yy, MM, dd, hh, mm;
            int key;
            if (s >= 0) { splitTime(s, yy, MM, dd, hh, mm); key = ymd(yy, MM, dd); }
            else { key = int(t->getEndDate() / 10000); } //무슨 용도지?
            byDate[key].push_back(t);
        }
        for (map<int, vector<Task*> >::iterator it = byDate.begin(); it != byDate.end(); ++it) {
            int dateKey = it->first;
            vector<Task*>& vec = it->second;
            cout << "\n[" << dateKey << "]\n";
            for (Task* t : vec) printTaskLine(t);
        }
        if (byDate.empty()) cout << "이번 주 일정 없음\n";
    }

    // 일 뷰: YYYY MM DD 입력 → 해당 일 작업 출력
    void TUI::showDay(int year, int month, int day) {
        int y=year, m=month, d=day;
        /*; cout << "연도 월 일 입력 (예: 2025 10 28): ";
        cin >> y >> m >> d;*/
        vector<Task*> tasks;
        if (!c->get_Day(tasks, y, m, d)) {
            cout << "해당 일 일정 없음\n"; return;
        }
        cout << "\n[" << y << "-" << m << "-" << d << "] 일정\n";
        for (Task* t : tasks) printTaskLine(t);
        if (tasks.empty()) cout << "일정 없음\n";
    }

    // 작업 추가
    void TUI::addTask() {
        string name; ll dur, due; int type;
        cout << "이름: ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        getline(cin, name);
        cout << "예상소요(분): "; cin >> dur;
        cout << "마감일(YYYYMMDDhhmm): "; cin >> due;
        cout << "타입(정수): "; cin >> type;
        if (c->addTask(name, dur, due, type))
            cout << "추가 완료\n";
        else cout << "추가 실패\n";
    }

    // 작업 수정
    void TUI::editTask() {
        int num; cout << "수정할 TaskNum: "; cin >> num;   //수정할 수 있는 task 목록을 출력
        string name; ll dur, due; int type;
        cout << "새 이름: ";
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        getline(cin, name);
        cout << "새 소요(분): "; cin >> dur;  
        cout << "새 마감(YYYYMMDDhhmm): "; cin >> due;
        cout << "새 타입: "; cin >> type;
        if (c->editTask(num, name, dur, due, type))
            cout << "수정 완료\n";
        else cout << "수정 실패\n";
    }

    // 작업 삭제
    void TUI::deleteTask() {
        int num; cout << "삭제할 TaskNum: "; cin >> num;  //수정할 수 있는 task 목록 출력
        if (c->deleteTask(num))
            cout << "삭제 완료\n";
        else cout << "삭제 실패\n";
    }

    // 통계 보기
    void TUI::showStatistics() {
        auto st = c->getStatistics();
        cout << "완료:" << st.finished_count << " / 미완료:" << st.unfinished_count
            << " / 남은 작업:" << st.to_do_count << "\n";
    }

    // 스케줄 간격 변경
    void TUI::changeInterval() {
        int k; cout << "새 interval(분): "; cin >> k;
        vector<Task*> q, f;
        if (c->changeInterval(k, q, f))
            cout << "간격 변경 및 재스케줄 완료. queued=" << q.size() << ", failed=" << f.size() << "\n";
        else cout << "변경 실패\n";
    }

} // namespace scheduler

/*
사용 예)
int main(){
    scheduler::TUI app;
    app.mainMenu();
}
*/
