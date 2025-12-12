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

// using namespace std;
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
        if (h < 0) return "--:--";
        ostringstream os;
        os << setw(2) << setfill('0') << h << ":" << setw(2) << setfill('0') << mm;
        return os.str();
    }

    static int weekdayOf(int y, int m, int d) { // 1=Mon..7=Sun
        std::tm tm{};
        tm.tm_year = y - 1900;
        tm.tm_mon  = m - 1;
        tm.tm_mday = d;
        std::mktime(&tm);
        int w = tm.tm_wday;
        if (w == 0) w = 7;
        return w;
    }

    static int lastDayOfMonth(int y, int m) {
        static int md[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
        if (m != 2) return md[m];
        if ((y % 400 == 0) || (y % 4 == 0 && y % 100 != 0)) return 29;
        return 28;
    }

    // 날짜/시간 입력 보조 함수
    static long long inputDateTime(const string& label) {
        int y, m, d, hh, mm;
        while (true) {
            cout << "\n>> [" << label << "] 날짜 및 시간 입력\n";
            cout << "   Format: YYYY MM DD HH MM (띄어쓰기로 구분)\n";
            cout << "   입력: ";
            
            if (cin >> y >> m >> d >> hh >> mm) {
                int ld = lastDayOfMonth(y, m);
                if (m >= 1 && m <= 12 && d >= 1 && d <= ld &&
                    hh >= 0 && hh <= 23 && mm >= 0 && mm <= 59) {
                    
                    return (long long)y * 100000000LL
                         + (long long)m * 1000000LL
                         + (long long)d * 10000LL
                         + (long long)hh * 100LL
                         + (long long)mm;
                } else {
                    cout << "   (!) 유효하지 않은 날짜/시간입니다.\n";
                }
            } else {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "   (!) 잘못된 입력 형식입니다. 숫자로 입력해주세요.\n";
            }
        }
    }

    // 일정 타입 안내
    static void printTaskTypeInfo() {
        cout << "\n-----------------------------\n";
        cout << " [일정 타입 코드]\n";
        cout << " 0 : 일반 과제 (Deadline)\n";
        cout << " 1 : 고정 일정 (수업/회의 등)\n";
        cout << " 2 : 기타\n";
        cout << "-----------------------------\n";
    }

    static void printTaskLine(Task* t) {
        long long s, e;
        t->getTime(s, e);

        string dueStr = "";
        if (t->getEndDate() > 0) {
            int y, m, d, hh, mm;
            splitTime(t->getEndDate(), y, m, d, hh, mm);
            dueStr = to_string(m) + "/" + to_string(d) + " " + to_string(hh) + ":" + (mm < 10 ? "0" : "") + to_string(mm);
        }

        cout << "  [" << setw(2) << t->getTaskNum() << "] "
             << (t->isfinished() ? "[v] " : "[ ] ")
             << left << setw(20) << t->getTaskName()
             << "| " << hhmm(s) << "~" << hhmm(e)
             << "| Type:" << t->getType()
             << "| Due: " << dueStr << "\n";
    }

    // ---- 시간 관련 TUI helpers ----

    void TUI::get_current_time(int& year, int& month, int& day) {
        std::time_t now_utc   = std::time(nullptr);
        std::time_t kst_epoch = now_utc + 9 * 60 * 60; 
        std::tm tm = *std::gmtime(&kst_epoch);
        year  = tm.tm_year + 1900;
        month = tm.tm_mon + 1;
        day   = tm.tm_mday;
    }

    void TUI::get_next_day(int& year, int& month, int& day, int offset) {
        struct tm timeinfo = { 0 };
        timeinfo.tm_year = year - 1900;
        timeinfo.tm_mon  = month - 1;
        timeinfo.tm_mday = day;
        time_t t = mktime(&timeinfo) + offset * (24 * 60 * 60);
        timeinfo = *std::localtime(&t); // localtime 사용 시 시스템 타임존 따름
        year  = timeinfo.tm_year + 1900;
        month = timeinfo.tm_mon + 1;
        day   = timeinfo.tm_mday;
    }

    void TUI::get_next_week(int& year, int& month, int& day, int offset) {
        get_next_day(year, month, day, offset * 7);
    }

    void TUI::get_next_month(int& year, int& month, int& day, int offset) {
        month += offset;
        while (month > 12) { month -= 12; year++; }
        while (month < 1)  { month += 12; year--; }
        int ld = lastDayOfMonth(year, month);
        if (day > ld) day = ld;
    }

    void TUI::get_first_day_of_week(int& year, int& month, int& day) {
        int w = weekdayOf(year, month, day); // 1=Mon..7=Sun
        get_next_day(year, month, day, -(w - 1));
    }

    // ---- TUI methods ----

    void TUI::mainMenu() {
        hideCursor();

        if (!c) c = new Calender();

        int year = 0, month = 0, day = 0;
        get_current_time(year, month, day);

        int last_choice = 1; // 1:Month, 2:Week, 3:Day, 0:Menu

        // 초기 진입 시 달력 표시
        system("cls");
        showMonth(year, month);

        while (true) {
            // 1. 달력/주간/일간 뷰 모드일 때 (방향키 입력 대기)
            if (last_choice >= 1 && last_choice <= 3) {
                cout << "\n[←/→: 날짜 이동]  [Other Key: 메인 메뉴]\n";
                
                int key = _getch();
                if (key == 224 || key == 0) { // 방향키 감지
                    key = _getch();
                    int offset = 0;
                    if (key == 75) offset = -1;      // Left
                    else if (key == 77) offset = 1;  // Right
                    
                    if (offset != 0) {
                        if (last_choice == 1) get_next_month(year, month, day, offset);
                        else if (last_choice == 2) get_next_week(year, month, day, offset);
                        else if (last_choice == 3) get_next_day(year, month, day, offset);
                    }
                    
                    // 화면 갱신
                    system("cls");
                    if (last_choice == 1) showMonth(year, month);
                    else if (last_choice == 2) showWeek(year, month, day);
                    else if (last_choice == 3) showDay(year, month, day);
                    continue; // 루프 처음으로 (계속 뷰 모드 유지)
                } else {
                    // 방향키가 아닌 키를 누르면 메인 메뉴 텍스트 모드로 전환
                    last_choice = 0;
                }
            }

            // 2. 메인 메뉴 텍스트 표시
            system("cls");
            cout << "\n===== SCHEDULER MAIN MENU =====\n";
            cout << " [1] 월별 보기 (Month View)\n";
            cout << " [2] 주별 보기 (Week View)\n";
            cout << " [3] 일별 보기 (Day View)\n";
            cout << " ---------------------------\n";
            cout << " [4] 작업 추가 (Add Task)\n";
            cout << " [5] 작업 수정 (Edit Task)\n";
            cout << " [6] 작업 삭제 (Delete Task)\n";
            cout << " [7] 통계 보기 (Statistics)\n";
            cout << " [8] 간격 변경 (Change Interval)\n";
            cout << " ---------------------------\n";
            cout << " [9] 고정 일정 추가 (Fixed Task)\n";
            cout << " [A] 작업 불가 시간 설정 (Set Blocked Time)\n";
            cout << " [0] 종료 (Exit)\n";
            cout << "===============================\n";
            cout << " 명령 선택 >> ";

            int cmd = -1;
            int ch = _getch(); // 키 입력 (화면에 안 보임)

            if (ch == '0') break;
            if (ch >= '1' && ch <= '9') cmd = ch - '0';
            if (ch == 'a' || ch == 'A') cmd = 10;

            system("cls"); // 메뉴 선택 후 화면 클리어

            // 선택에 따른 동작 수행
            switch (cmd) {
            case 1: // Month View
                showMonth(year, month);
                last_choice = 1;
                break;
            case 2: // Week View
                showWeek(year, month, day);
                last_choice = 2;
                break;
            case 3: // Day View
                showDay(year, month, day);
                last_choice = 3;
                break;
            
            case 4: // Add Task
                showCursor();
                addTask();
                hideCursor();
                cout << "\n(아무 키나 누르면 메뉴로 돌아갑니다)";
                _getch();
                last_choice = 0; // 작업 후엔 메뉴로
                break;
            case 5: // Edit Task
                showCursor();
                editTask();
                hideCursor();
                cout << "\n(아무 키나 누르면 메뉴로 돌아갑니다)";
                _getch();
                last_choice = 0;
                break;
            case 6: // Delete Task
                showCursor();
                deleteTask();
                hideCursor();
                cout << "\n(아무 키나 누르면 메뉴로 돌아갑니다)";
                _getch();
                last_choice = 0;
                break;
            case 7: // Statistics
                showStatistics();
                cout << "\n(아무 키나 누르면 메뉴로 돌아갑니다)";
                _getch();
                last_choice = 0;
                break;
            case 8: // Change Interval
                showCursor();
                changeInterval();
                hideCursor();
                cout << "\n(아무 키나 누르면 메뉴로 돌아갑니다)";
                _getch();
                last_choice = 0;
                break;
            case 9: // Add Fixed Task
                showCursor();
                addFixedTask();
                hideCursor();
                cout << "\n(아무 키나 누르면 메뉴로 돌아갑니다)";
                _getch();
                last_choice = 0;
                break;
            case 10: // Set Uninterrupted Time
                showCursor();
                setUninterruptedTime();
                hideCursor();
                cout << "\n(아무 키나 누르면 메뉴로 돌아갑니다)";
                _getch();
                last_choice = 0;
                break;
            default:
                // 잘못된 입력 등은 무시하고 다시 메뉴
                last_choice = 0; 
                break;
            }
        }
    }

    void TUI::showMonth(int year, int month) {
        int y = year, m = month;
        vector<Task*> tasks;
        int dummyDay = 1;
        
        // 데이터가 없어도 달력은 그려야 하므로 반환값 무시하고 진행
        c->get_Month(tasks, y, m, dummyDay);

        map<int, int> cnt;
        for (Task* t : tasks) {
            long long s, e;
            t->getTime(s, e);
            int dateVal;
            if (s >= 0) {
                int yy, MM, dd, hh, mm;
                splitTime(s, yy, MM, dd, hh, mm);
                dateVal = yy * 10000 + MM * 100 + dd;
            } else {
                dateVal = (int)(t->getEndDate() / 10000); // Deadline 기준
            }
            int d = dateVal % 100;
            cnt[d]++;
        }

        int firstW = weekdayOf(y, m, 1);
        int lastD  = lastDayOfMonth(y, m);

        cout << "\n   <<<  " << y << "년 " << m << "월  >>>\n";
        cout << "-----------------------------\n";
        cout << " Mo Tu We Th Fr Sa Su\n";

        int col = 1;
        for (int i = 1; i < firstW; ++i) {
            cout << "   ";
            col++;
        }

        for (int d = 1; d <= lastD; ++d) {

            // [수정] 작업이 있으면 "숫자*", 없으면 " 숫자" 출력
            if (cnt[d] > 0) {
                cout << setw(3) << d << "*"; // 예: " 5*"
            }
            else {
                cout << setw(3) << d;        // 예: "  5"
            }

            if (col % 7 == 0) cout << "\n";
            col++;
        }
        if ((col - 1) % 7 != 0) cout << "\n";
        cout << "-----------------------------\n";
        
        if (tasks.empty()) {
            cout << " * 이 달에는 등록된 일정이 없습니다.\n";
        } else {
            cout << " * 총 " << tasks.size() << "개의 일정이 있습니다. (상세보기: 일간/주간)\n";
        }
    }

    void TUI::showWeek(int year, int month, int day) {
        int y = year, m = month, d = day;
        get_first_day_of_week(y, m, d); // 해당 주의 월요일로 이동

        cout << "\n   <<< 주간 일정 (" << m << "/" << d << " 주) >>>\n";

        vector<Task*> tasks;
        c->get_Week(tasks, y, m, d);

        map<int, vector<Task*>> byDate;
        for (Task* t : tasks) {
            long long s, e;
            t->getTime(s, e);
            int key;
            if (s >= 0) {
                int yy, MM, dd, hh, mm;
                splitTime(s, yy, MM, dd, hh, mm);
                key = ymd(yy, MM, dd);
            } else {
                key = int(t->getEndDate() / 10000);
            }
            byDate[key].push_back(t);
        }

        // 월~일 순회하며 출력
        int curY = y, curM = m, curD = d;
        for (int i = 0; i < 7; i++) {
            int key = ymd(curY, curM, curD);
            cout << " [" << curM << "/" << curD << "] ";
            
            if (byDate[key].empty()) {
                cout << "-\n";
            } else {
                cout << "\n";
                for (Task* t : byDate[key]) {
                    printTaskLine(t);
                }
            }
            get_next_day(curY, curM, curD, 1);
        }
    }

    void TUI::showDay(int year, int month, int day) {
        int y = year, m = month, d = day;
        vector<Task*> tasks;
        c->get_Day(tasks, y, m, d);

        cout << "\n   <<< " << y << "-" << m << "-" << d << " 상세 일정 >>>\n";
        cout << "------------------------------------------------\n";
        if (tasks.empty()) {
            cout << "  (일정이 없습니다)\n";
        } else {
            for (Task* t : tasks) printTaskLine(t);
        }
        cout << "------------------------------------------------\n";
    }

    // ---- 작업 관련 기능 ----

    void TUI::addTask() {
        cout << "\n[새 작업 추가]\n";
        string name;
        ll dur, due;
        int type;

        cout << "이름: ";
        getline(cin, name);

        cout << "예상 소요(분): ";
        while (!(cin >> dur)) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "숫자만 입력하세요: ";
        }

        due = inputDateTime("마감 기한");

        printTaskTypeInfo();
        cout << "타입 번호 입력: ";
        cin >> type;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (c->addTask(name, dur, due, type))
            cout << ">> 추가 성공!\n";
        else
            cout << ">> 추가 실패 (중복되거나 유효하지 않음)\n";
    }

    void TUI::editTask() {
        cout << "\n[작업 수정]\n";

        cout << "------------------------------------------------\n";
        cout << " 현재 대기열(Queue)에 있는 작업 목록\n";
        cout << "------------------------------------------------\n";

        vector<Task*> queuedList;
        c->getQueuedTasks(queuedList); // [변경] getAllTasks -> getQueuedTasks

        if (queuedList.empty()) {
            cout << "  (수정할 수 있는 대기 중인 작업이 없습니다)\n";
            cout << "------------------------------------------------\n";
            // 작업이 없으면 바로 돌아가는 게 좋습니다.
            return;
        }

        for (Task* t : queuedList) {
            printTaskLine(t);
        }
        cout << "------------------------------------------------\n";

        int num;
        cout << "수정할 Task 번호: ";
        while (!(cin >> num)) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "숫자만 입력하세요: ";
        }
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string name;
        ll dur, due;
        int type;

        cout << "새 이름: ";
        getline(cin, name);

        cout << "새 소요(분): ";
        cin >> dur;

        due = inputDateTime("새 마감 기한");

        printTaskTypeInfo();
        cout << "새 타입 번호: ";
        cin >> type;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (c->editTask(num, name, dur, due, type))
            cout << ">> 수정 성공!\n";
        else
            cout << ">> 수정 실패\n";
    }

    void TUI::deleteTask() {
        cout << "\n[작업 삭제]\n";
        int num;
        cout << "삭제할 Task 번호: ";
        cin >> num;
        // 다음 getline 대비 버퍼 정리
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (c->deleteTask(num))
            cout << ">> 삭제 성공!\n";
        else
            cout << ">> 삭제 실패 (존재하지 않는 번호)\n";
    }

    void TUI::showStatistics() {
        system("cls");
        cout << "\n[ 통계 정보 ]\n";
        auto st = c->getStatistics();
        cout << " - 완료된 작업 : " << st.finished_count << "\n";
        cout << " - 미완료 작업 : " << st.unfinished_count << "\n";
        cout << " - 남은 작업   : " << st.to_do_count << "\n";
    }

    void TUI::changeInterval() {
        cout << "\n[ 스케줄링 간격 변경 ]\n";
        int k;
        cout << "새 분 단위 간격 (예: 30): ";
        cin >> k;
        // 다음 getline 대비 버퍼 정리
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        vector<Task*> q, f;
        if (c->changeInterval(k, q, f)) {
            cout << ">> 변경 및 재스케줄링 완료.\n";
            cout << "   (재배치 대기: " << q.size() << ", 배치 실패: " << f.size() << ")\n";
        } else {
            cout << ">> 변경 실패\n";
        }
    }

    // 요구사항: 고정 일정 추가
    void TUI::addFixedTask() {
        cout << "\n[ 고정 일정(수업 등) 추가 ]\n";
        
        string name;
        cout << "일정 이름: ";
        getline(cin, name);

        ll start = inputDateTime("시작 시간");
        ll end   = inputDateTime("종료 시간");

        if (start >= end) {
            cout << ">> 오류: 종료 시간이 시작 시간보다 빨라야 합니다.\n";
            return;
        }

        int type;
        printTaskTypeInfo();
        cout << "타입 번호 입력(주로 1): ";
        cin >> type;
        // 다음 getline 대비 버퍼 정리
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (c->addFixedTask(name, start, end, type)) 
            cout << ">> 고정 일정 추가 성공!\n";
        else
            cout << ">> 고정 일정 추가 실패 (시간 충돌 등)\n";
    }

    // 요구사항: 작업 불가 시간대 추가
    void TUI::setUninterruptedTime() {
        cout << "\n[ 작업 불가 시간대 설정 ]\n";
        cout << " (잠자는 시간, 식사 시간 등 작업을 할당하지 않을 구간)\n";

        // 주의: Scheduler 로직에 따르면 YYYYMMDDHHMM이 아니라 HHMM(시간/분)만 필요할 수 있습니다.
        // 현재 로직대로라면 날짜까지 포함된 큰 숫자가 들어가게 됩니다. 
        // 만약 매일 반복되는 시간을 원하신다면 입력 방식을 변경해야 할 수도 있습니다.
        ll start = inputDateTime("시작 시간");
        ll end = inputDateTime("종료 시간");

        if (start >= end) {
            cout << ">> 오류: 시간 범위가 잘못되었습니다.\n";
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // [수정된 부분] if문을 제거하고 직접 호출
        c->setUninterruptedTime(start, end);
        cout << ">> 설정 완료!\n";
    }

} // namespace scheduler

/*
사용 예)
int main(){
    scheduler::TUI app;
    app.mainMenu();
}
*/

