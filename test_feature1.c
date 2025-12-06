#include "types.h"
#include "user.h"
#include "stat.h"
#include "pstat.h"

// burn CPU
void burn()
{
    volatile int x = 0;
    for (long i = 0; i < 20000000; i++)
        x++;
}

// print pstat cleanly
void print_stats(char *label)
{
    struct pstat st;
    update_ticket_status(&st);

    printf(1, "%s\n", label);
    printf(1, " PID: %d\n", st.pid);
    printf(1, " Base Tickets: %d\n", st.base_tickets);
    printf(1, " Accumulated:  %d\n", st.acc_tickets);
    printf(1, " Exchanged:    %d\n", st.exch_tickets);
    printf(1, " Dependency:   %d\n", st.dependency);
    printf(1, " CPU Ticks:    %d\n", st.total_ticks);
    printf(1, "----------------------------------------\n");
}

int main()
{
    printf(1, "\n========================================\n");
    printf(1, "       FEATURE 1 TEST — LOTTERY CPU     \n");
    printf(1, "========================================\n\n");

    // -------------------------------------------------------------
    // PART 1: Base Ticket Test
    // -------------------------------------------------------------
    printf(1, "[1] Testing base ticket assignment...\n");

    set_base_tickets(10);
    burn();
    print_stats("After running with 10 tickets:");

    set_base_tickets(30);
    burn();
    print_stats("After increasing tickets to 30:");

    // -------------------------------------------------------------
    // PART 2: Reward Ticket Test
    // -------------------------------------------------------------
    printf(1, "[2] Testing reward ticket accumulation...\n");

    set_base_tickets(10);
    for (int i = 0; i < 5; i++) {
        grant_reward(5);
        sleep(10);
        print_stats("After grant_reward(5):");
    }

    // -------------------------------------------------------------
    // PART 3: Fairness with forked processes
    // -------------------------------------------------------------
    printf(1, "[3] Testing fairness across processes...\n");
    printf(1, "Launching 3 children with ticket ratios 3:2:1\n\n");

    int tickets[3] = {30, 20, 10};
    int pids[3];

    for (int i = 0; i < 3; i++) {
        pids[i] = fork();
        if (pids[i] == 0) {
            set_base_tickets(tickets[i]);
            burn();
            print_stats("Child finished:");
            exit();
        }
    }

    for (int i = 0; i < 3; i++)
        wait();

    printf(1, "[Fairness Test Completed]\n\n");

    // -------------------------------------------------------------
    // PART 4: ticket exchange (optional)
    // -------------------------------------------------------------
    printf(1, "[4] Testing ticket exchange (if implemented)...\n");

    int pid = fork();
    if (pid == 0) {
        set_base_tickets(10);
        burn();
        print_stats("Child before donation:");
        exit();
    }

    sleep(20);

    set_base_tickets(5);
    grant_reward(20);     // simulate donation
    print_stats("Parent after donating 20 tickets:");

    wait();

    printf(1, "\n========================================\n");
    printf(1, " FEATURE 1 TEST COMPLETE — SEE RESULTS  \n");
    printf(1, "========================================\n\n");

    exit();
}
