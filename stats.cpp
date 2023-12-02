#include "stats.h"

void Stats::l1_hit()
{
    hitsL1++;
}

void Stats::l2_hit()
{
    hitsL2++;
}

void Stats::victim_hit()
{
    hitsVic++;
}

void Stats::l1_miss()
{
    missesL1++;
}

void Stats::l2_miss()
{
    missesL2++;
}

void Stats::victim_miss()
{
    missesVic++;
}

double Stats::L1_miss_rate()
{
    return static_cast<double>(missesL1) / (missesL1 + hitsL1);
}

double Stats::L2_miss_rate()
{
    return static_cast<double>(missesL2) / (missesL2 + hitsL2);
}

double Stats::Victim_miss_rate()
{
    return static_cast<double>(missesVic) / (missesVic + hitsVic);
}

double Stats::AAT()
{
    return 1 + L1_miss_rate() * (1 + Victim_miss_rate() * (8 + L2_miss_rate() * 100));
}
