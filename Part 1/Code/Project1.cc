#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <tgmath.h>

using namespace std;

// BEGIN DEFINITIONS
#define IN_LEN 100

#define NO_TEAMS 32       // Number of NFL teams
#define TEAM_NAME_LEN 25  // Maximum team name string length
#define TOP_LEN 6         // Maximum time of possession string length

struct team_stats {
	char team_name[ TEAM_NAME_LEN];  // Name of NFL team
	int games;                        // Number of games played in the season
	float pts_per_game;               // Points per game
	int total_points;                 // Total points
	int scrimmage_plays;              // Scrimmage plays
	float yds_per_game;               // Yards per game
	float yds_per_play;               // Yards per play
	float first_per_game;             // First downs per game
	int third_md;                     // Third down conversions
	int third_att;                    // Third down attempts
	int third_pct;                    // Third down percentage
	int fourth_md;                    // Fourth down conversions
	int fourth_att;                   // Fourth down attempts
	int fourth_pct;                   // Fourth down percentage
	int penalties;                    // Number of penalties
	int pen_yds;                      // Penalty yards
	char top_per_game[TOP_LEN];       // Time of possession per game
	int fum;                          // Number of fumbles
	int lost;                         // Fumbles lost
	int to;                           // Turnover ratio
};

struct annual_stats {
	int year;
	struct team_stats teams[NO_TEAMS];
};

typedef struct {
	struct annual_stats *stats;
	int years;
	char *field;
	int *indices;
	int *spots;
} ALL_stats;

// BEGIN FUNCTIONS
void exchange(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

int str_len(char *str) {
	int n = 0;

	while (*str != '\t' && *(str++) != '\0')
		++n;

	return n;
}

void bsort(ALL_stats *stats) {
	int i = 0;
	int n = NO_TEAMS * stats->years - 1;
	bool swapped = true;

	while(swapped)
	{
		swapped = false;

		for (i = 0; i < n; i++) {
			if (*(stats->spots + i) > *(stats->spots + i + 1)) {

				exchange(stats->spots + i, stats->spots + i + 1);

				exchange(stats->indices + i, stats->indices + i + 1);
				swapped = true;
			}
		}
		n--;
	}
}

void qsort(ALL_stats *stats, int left, int right)
{
    int i = left, j = right;
    int pivot = *(stats->spots + (left + right) / 2);

    while (i <= j) {
          while (*(stats->spots + i) < pivot)
                i++;
          while (*(stats->spots + j) > pivot)
                j--;
          if (i <= j) {
				exchange(stats->spots + i, stats->spots + j);
				exchange(stats->indices + i, stats->indices + j);
                i++;
                j--;
          }
    }

    if (left < j)
          qsort(stats, left, j);
    if (i < right)
          qsort(stats, i, right);
}

void pmax()
{

}

void pmin()
{

}

int process(char *in) {
	int m = 0;

	for (int i = 1; i <= 12; i++) {
		m = m << 5;

		while (!((*in >= 'A' && *in <= 'Z') || (*in >= 'a' && *in <= 'z')
				|| *in != '\0' && (*in >= '0' && *in <= '9')))
			in++;

		if (*in != '\0') {
			int n = 0;

			if (*in >= '0' && *in <= '9')
				n = *in - '0' + 1;
			else if (*in >= 'a' && *in <= 'z')
				n = *in - 'a' + 1;
			else if (*in >= 'A' && *in <= 'Z')
				n = *in - 'A' + 1;

			m += n;
			in++;
		}
	}

	return m;
}

struct annual_stats *in() {
	int years;

	cin >> years;

	struct annual_stats *stats = new annual_stats[years];

	for (int i = 0; i < years; i++) {
		cin >> stats[i].year;
		cin.ignore();

		for (int j = 0; j < NO_TEAMS; j++) {
			char *input = new char[IN_LEN];
			char *beginning = input;

			cin.getline(input, IN_LEN);

			int x = 0;

			while (*input != '\t') {
				stats[i].teams[j].team_name[x++] = *(input++);
			}

			++input;

			int length = str_len(input);
			stats[i].teams[j].games = atoi(input);
			input += length + 1;

			length = str_len(input);
			stats[i].teams[j].pts_per_game = atof(input);
			input += length + 1;

			length = str_len(input);
			stats[i].teams[j].total_points = atoi(input);
			input += length + 1;

			length = str_len(input);
			stats[i].teams[j].scrimmage_plays = atoi(input);
			input += length + 1;

			length = str_len(input);
			stats[i].teams[j].yds_per_game = atof(input);
			input += length + 1;

			length = str_len(input);
			stats[i].teams[j].yds_per_play = atof(input);
			input += length + 1;

			length = str_len(input);
			stats[i].teams[j].first_per_game = atof(input);
			input += length + 1;

			length = str_len(input);
			stats[i].teams[j].third_md = atoi(input);
			input += length + 1;

			length = str_len(input);
			stats[i].teams[j].third_att = atoi(input);
			input += length + 1;

			length = str_len(input);
			stats[i].teams[j].third_pct = atoi(input);
			input += length + 1;

			length = str_len(input);
			stats[i].teams[j].fourth_md = atoi(input);
			input += length + 1;

			length = str_len(input);
			stats[i].teams[j].fourth_att = atoi(input);
			input += length + 1;

			length = str_len(input);
			stats[i].teams[j].fourth_pct = atoi(input);
			input += length + 1;

			length = str_len(input);
			stats[i].teams[j].penalties = atoi(input);
			input += length + 1;

			length = str_len(input);
			stats[i].teams[j].pen_yds = atoi(input);
			input += length + 1;

			int y = 0;

			while (*input != '\t') {
				stats[i].teams[j].top_per_game[y++] = *(input++);
			}

			++input;

			length = str_len(input);
			stats[i].teams[j].fum = atoi(input);
			input += length + 1;

			length = str_len(input);
			stats[i].teams[j].lost = atoi(input);
			input += length + 1;

			length = str_len(input);
			stats[i].teams[j].to = atoi(input);
			input += length + 1;

			delete[] beginning;
		}
	}

	return stats;
}

ALL_stats *compare(struct annual_stats *stats, int years, char *field,
		int constant) {

	ALL_stats *value = new ALL_stats;
	value->years = years;
	value->field = field;
	value->indices = new int[years * NO_TEAMS];
	value->spots = new int[years * NO_TEAMS];
	value->stats = stats;

	int *z = value->indices;

	for (int i = 0; i < years * NO_TEAMS; i++)
		*(z++) = i;

	int *current = value->spots;

	if (0 == strcmp(field, "team_name"))
		for (int i = 0; i < NO_TEAMS * years; i++)
			*(current++) =
					constant
							* process(
									(stats + (i / NO_TEAMS))->teams[i % NO_TEAMS].team_name);

	else if (0 == strcmp(field, "games"))
		for (int i = 0; i < NO_TEAMS * years; i++)
			*(current++) = constant
					* (int) (stats + (i / NO_TEAMS))->teams[i % NO_TEAMS].games;

	else if (0 == strcmp(field, "pts_per_game"))
		for (int i = 0; i < NO_TEAMS * years; i++)
			*(current++) =
					constant
							* (int) (10
									* (stats + (i / NO_TEAMS))->teams[i
											% NO_TEAMS].pts_per_game);

	else if (0 == strcmp(field, "total_points"))
		for (int i = 0; i < NO_TEAMS * years; i++)
			*(current++) =
					constant
							* (int) (stats + (i / NO_TEAMS))->teams[i % NO_TEAMS].total_points;

	else if (0 == strcmp(field, "scrimmage_plays"))
		for (int i = 0; i < NO_TEAMS * years; i++)
			*(current++) =
					constant
							* (int) (stats + (i / NO_TEAMS))->teams[i % NO_TEAMS].scrimmage_plays;

	else if (0 == strcmp(field, "yds_per_game"))
		for (int i = 0; i < NO_TEAMS * years; i++)
			*(current++) =
					constant
							* (int) (10
									* (stats + (i / NO_TEAMS))->teams[i
											% NO_TEAMS].yds_per_game);

	else if (0 == strcmp(field, "yds_per_play"))
		for (int i = 0; i < NO_TEAMS * years; i++)
			*(current++) =
					constant
							* (int) (10
									* (stats + (i / NO_TEAMS))->teams[i
											% NO_TEAMS].yds_per_play);

	else if (0 == strcmp(field, "first_per_game"))
		for (int i = 0; i < NO_TEAMS * years; i++)
			*(current++) =
					constant
							* (int) (10
									* (stats + (i / NO_TEAMS))->teams[i
											% NO_TEAMS].first_per_game);

	else if (0 == strcmp(field, "third_md"))
		for (int i = 0; i < NO_TEAMS * years; i++)
			*(current++) =
					constant
							* (int) (stats + (i / NO_TEAMS))->teams[i % NO_TEAMS].third_md;

	else if (0 == strcmp(field, "third_att"))
		for (int i = 0; i < NO_TEAMS * years; i++)
			*(current++) =
					constant
							* (int) (stats + (i / NO_TEAMS))->teams[i % NO_TEAMS].third_att;

	else if (0 == strcmp(field, "third_pct"))
		for (int i = 0; i < NO_TEAMS * years; i++)
			*(current++) =
					constant
							* (int) (stats + (i / NO_TEAMS))->teams[i % NO_TEAMS].third_pct;

	else if (0 == strcmp(field, "fourth_md"))
		for (int i = 0; i < NO_TEAMS * years; i++)
			*(current++) =
					constant
							* (int) (stats + (i / NO_TEAMS))->teams[i % NO_TEAMS].fourth_md;

	else if (0 == strcmp(field, "fourth_att"))
		for (int i = 0; i < NO_TEAMS * years; i++)
			*(current++) =
					constant
							* (int) (stats + (i / NO_TEAMS))->teams[i % NO_TEAMS].fourth_att;

	else if (0 == strcmp(field, "fourth_pct"))
		for (int i = 0; i < NO_TEAMS * years; i++)
			*(current++) =
					constant
							* (int) (stats + (i / NO_TEAMS))->teams[i % NO_TEAMS].fourth_pct;

	else if (0 == strcmp(field, "penalties"))
		for (int i = 0; i < NO_TEAMS * years; i++)
			*(current++) =
					constant
							* (int) (stats + (i / NO_TEAMS))->teams[i % NO_TEAMS].penalties;

	else if (0 == strcmp(field, "pen_yds"))
		for (int i = 0; i < NO_TEAMS * years; i++)
			*(current++) =
					constant
							* (int) (stats + (i / NO_TEAMS))->teams[i % NO_TEAMS].pen_yds;

	else if (0 == strcmp(field, "top_per_game"))
		for (int i = 0; i < NO_TEAMS * years; i++)
			*(current++) =
					constant
							* process(
									(stats + (i / NO_TEAMS))->teams[i % NO_TEAMS].top_per_game);

	else if (0 == strcmp(field, "fum"))
		for (int i = 0; i < NO_TEAMS * years; i++)
			*(current++) = constant
					* (int) (stats + (i / NO_TEAMS))->teams[i % NO_TEAMS].fum;

	else if (0 == strcmp(field, "lost"))
		for (int i = 0; i < NO_TEAMS * years; i++)
			*(current++) = constant
					* (int) (stats + (i / NO_TEAMS))->teams[i % NO_TEAMS].lost;

	else if (0 == strcmp(field, "to"))
		for (int i = 0; i < NO_TEAMS * years; i++)
			*(current++) = constant
					* (int) (stats + (i / NO_TEAMS))->teams[i % NO_TEAMS].to;

	else
		cout << endl << "FIELD NOT FOUND: " << field << endl;

	return value;
}

void median(ALL_stats *stats) {
	cout << "Median " << stats->field << endl;

	int i = *(stats->indices + (NO_TEAMS + 1) / 2);

	if (0 == strcmp(stats->field, "team_name"))
		cout << stats->stats->teams[i].team_name;

	else if (0 == strcmp(stats->field, "games"))
		cout << stats->stats->teams[i].games;

	else if (0 == strcmp(stats->field, "pts_per_game"))
		cout << stats->stats->teams[i].pts_per_game;

	else if (0 == strcmp(stats->field, "total_points"))
		cout << stats->stats->teams[i].total_points;

	else if (0 == strcmp(stats->field, "scrimmage_plays"))
		cout << stats->stats->teams[i].scrimmage_plays;

	else if (0 == strcmp(stats->field, "yds_per_game"))
		cout << stats->stats->teams[i].yds_per_game;

	else if (0 == strcmp(stats->field, "yds_per_play"))
		cout << stats->stats->teams[i].yds_per_play;

	else if (0 == strcmp(stats->field, "first_per_game"))
		cout << stats->stats->teams[i].first_per_game;

	else if (0 == strcmp(stats->field, "third_md"))
		cout << stats->stats->teams[i].third_md;

	else if (0 == strcmp(stats->field, "third_att"))
		cout << stats->stats->teams[i].third_att;

	else if (0 == strcmp(stats->field, "third_pct"))
		cout << stats->stats->teams[i].third_pct;

	else if (0 == strcmp(stats->field, "fourth_md"))
		cout << stats->stats->teams[i].fourth_md;

	else if (0 == strcmp(stats->field, "fourth_att"))
		cout << stats->stats->teams[i].fourth_att;

	else if (0 == strcmp(stats->field, "fourth_pct"))
		cout << stats->stats->teams[i].fourth_pct;

	else if (0 == strcmp(stats->field, "penalties"))
		cout << stats->stats->teams[i].penalties;

	else if (0 == strcmp(stats->field, "pen_yds"))
		cout << stats->stats->teams[i].pen_yds;

	else if (0 == strcmp(stats->field, "top_per_game"))
		cout << stats->stats->teams[i].top_per_game;

	else if (0 == strcmp(stats->field, "fum"))
		cout << stats->stats->teams[i].fum;

	else if (0 == strcmp(stats->field, "lost"))
		cout << stats->stats->teams[i].lost;

	else if (0 == strcmp(stats->field, "to"))
		cout << stats->stats->teams[i].to;

	else
		cout << "FIELD NOT FOUND: " << stats->field;

	cout << '\n' << '\n';
}

void min_max(ALL_stats *stats, bool a) {
	cout << "TEAM" << "\t\t\t" << (a ? "Min " : "Max ");

	cout << stats->field << endl;

	int o = 0;
	bool x = false;

	do {
		x = false;
		struct team_stats current = stats->stats->teams[*(stats->indices + o)];
		struct team_stats next = stats->stats->teams[*(stats->indices + o + 1)];

		cout << current.team_name << '\t';

		if (str_len(current.team_name) < 16)
			cout << '\t';

		if (0 == strcmp(stats->field, "games")) {
			if (o < (NO_TEAMS - 1) && current.games == next.games)
				x = true;
			cout << current.games;
		}

		else if (0 == strcmp(stats->field, "pts_per_game")) {
			if (o < (NO_TEAMS - 1) && current.pts_per_game == next.pts_per_game)
				x = true;
			cout << current.pts_per_game;
		}

		else if (0 == strcmp(stats->field, "total_points")) {
			if (o < (NO_TEAMS - 1) && current.total_points == next.total_points)
				x = true;
			cout << current.total_points;
		}

		else if (0 == strcmp(stats->field, "scrimmage_plays")) {
			if (o < (NO_TEAMS - 1)
					&& current.scrimmage_plays == next.scrimmage_plays)
				x = true;
			cout << current.scrimmage_plays;
		}

		else if (0 == strcmp(stats->field, "yds_per_game")) {
			if (o < (NO_TEAMS - 1) && current.yds_per_game == next.yds_per_game)
				x = true;
			cout << current.yds_per_game;
		}

		else if (0 == strcmp(stats->field, "yds_per_play")) {
			if (o < (NO_TEAMS - 1) && current.yds_per_play == next.yds_per_play)
				x = true;
			cout << current.yds_per_play;
		}

		else if (0 == strcmp(stats->field, "first_per_game")) {
			if (o < (NO_TEAMS - 1)
					&& current.first_per_game == next.first_per_game)
				x = true;
			cout << current.first_per_game;
		}

		else if (0 == strcmp(stats->field, "third_md")) {
			if (o < (NO_TEAMS - 1) && current.third_md == next.third_md)
				x = true;
			cout << current.third_md;
		}

		else if (0 == strcmp(stats->field, "third_att")) {
			if (o < (NO_TEAMS - 1) && current.third_att == next.third_att)
				x = true;
			cout << current.third_att;
		}

		else if (0 == strcmp(stats->field, "third_pct")) {
			if (o < (NO_TEAMS - 1) && current.third_pct == next.third_pct)
				x = true;
			cout << current.third_pct;
		}

		else if (0 == strcmp(stats->field, "fourth_md")) {
			if (o < (NO_TEAMS - 1) && current.fourth_md == next.fourth_md)
				x = true;
			cout << current.fourth_md;
		}

		else if (0 == strcmp(stats->field, "fourth_att")) {
			if (o < (NO_TEAMS - 1) && current.fourth_att == next.fourth_att)
				x = true;
			cout << current.fourth_att;
		}

		else if (0 == strcmp(stats->field, "fourth_pct")) {
			if (o < (NO_TEAMS - 1) && current.fourth_pct == next.fourth_pct)
				x = true;
			cout << current.fourth_pct;
		}

		else if (0 == strcmp(stats->field, "penalties")) {
			if (o < (NO_TEAMS - 1) && current.penalties == next.penalties)
				x = true;
			cout << current.penalties;
		}

		else if (0 == strcmp(stats->field, "pen_yds")) {
			if (o < (NO_TEAMS - 1) && current.pen_yds == next.pen_yds)
				x = true;
			cout << current.pen_yds;
		}

		else if (0 == strcmp(stats->field, "top_per_game")) {
			if (0 == strcmp(current.top_per_game, next.top_per_game))
				x = true;
			cout << current.top_per_game;
		}

		else if (0 == strcmp(stats->field, "fum")) {
			if (o < (NO_TEAMS - 1) && current.fum == next.fum)
				x = true;
			cout << current.fum;
		}

		else if (0 == strcmp(stats->field, "lost")) {
			if (o < (NO_TEAMS - 1) && current.lost == next.lost)
				x = true;
			cout << current.lost;
		}

		else if (0 == strcmp(stats->field, "to")) {
			if (o < (NO_TEAMS - 1) && current.to == next.to)
				x = true;
			cout << current.to;
		}

		else if (0 != strcmp(stats->field, "team_name"))
			cout << "FIELD NOT FOUND: " << stats->field;

		o++;

		cout << '\n';
	} while (x);

	cout << '\n';
}

void average(ALL_stats *stats) {
	cout << "Average " << stats->field << endl;

	float sum = 0.0;

	for (int i = 0; i < NO_TEAMS; i++) {
		if (0 == strcmp(stats->field, "games"))
			sum += stats->stats->teams[i].games;

		else if (0 == strcmp(stats->field, "pts_per_game"))
			sum += stats->stats->teams[i].pts_per_game;

		else if (0 == strcmp(stats->field, "total_points"))
			sum += stats->stats->teams[i].total_points;

		else if (0 == strcmp(stats->field, "scrimmage_plays"))
			sum += stats->stats->teams[i].scrimmage_plays;

		else if (0 == strcmp(stats->field, "yds_per_game"))
			sum += stats->stats->teams[i].yds_per_game;

		else if (0 == strcmp(stats->field, "yds_per_play"))
			sum += stats->stats->teams[i].yds_per_play;

		else if (0 == strcmp(stats->field, "first_per_game"))
			sum += stats->stats->teams[i].first_per_game;

		else if (0 == strcmp(stats->field, "third_md"))
			sum += stats->stats->teams[i].third_md;

		else if (0 == strcmp(stats->field, "third_att"))
			sum += stats->stats->teams[i].third_att;

		else if (0 == strcmp(stats->field, "third_pct"))
			sum += stats->stats->teams[i].third_pct;

		else if (0 == strcmp(stats->field, "fourth_md"))
			sum += stats->stats->teams[i].fourth_md;

		else if (0 == strcmp(stats->field, "fourth_att"))
			sum += stats->stats->teams[i].fourth_att;

		else if (0 == strcmp(stats->field, "fourth_pct"))
			sum += stats->stats->teams[i].fourth_pct;

		else if (0 == strcmp(stats->field, "penalties"))
			sum += stats->stats->teams[i].penalties;

		else if (0 == strcmp(stats->field, "pen_yds"))
			sum += stats->stats->teams[i].pen_yds;

		else if (0 == strcmp(stats->field, "fum"))
			sum += stats->stats->teams[i].fum;

		else if (0 == strcmp(stats->field, "lost"))
			sum += stats->stats->teams[i].lost;

		else if (0 == strcmp(stats->field, "to"))
			sum += stats->stats->teams[i].to;

		else
			cout << "FIELD NOT FOUND: " << stats->field;
	}

	float avg = sum / NO_TEAMS;

	cout << avg << '\n' << '\n';
}

void out(ALL_stats *stats) {
	cout << "TEAM" << "\t\t\t";

	if (0 != strcmp(stats->field, "team_name")) {
		cout << stats->field;
		if (str_len(stats->field) < 11)
			cout << '\t';
	}

	if (stats->years >= 2)
		cout << '\t' << "YEAR" << '\t';

	cout << '\n';

	int *index = stats->indices;
	for (int i = 0; i < (NO_TEAMS * stats->years); i++) {

		struct team_stats team =
				(stats->stats + (*index / NO_TEAMS))->teams[*index % 32];

		cout << team.team_name << '\t';

		if (str_len(team.team_name) < 16)
			cout << '\t';

		if (0 == strcmp(stats->field, "games"))
			cout << team.games;

		else if (0 == strcmp(stats->field, "pts_per_game"))
			cout << team.pts_per_game;

		else if (0 == strcmp(stats->field, "total_points"))
			cout << team.total_points;

		else if (0 == strcmp(stats->field, "scrimmage_plays"))
			cout << team.scrimmage_plays;

		else if (0 == strcmp(stats->field, "yds_per_game"))
			cout << team.yds_per_game;

		else if (0 == strcmp(stats->field, "yds_per_play"))
			cout << team.yds_per_play;

		else if (0 == strcmp(stats->field, "first_per_game"))
			cout << team.first_per_game;

		else if (0 == strcmp(stats->field, "third_md"))
			cout << team.third_md;

		else if (0 == strcmp(stats->field, "third_att"))
			cout << team.third_att;

		else if (0 == strcmp(stats->field, "third_pct"))
			cout << team.third_pct;

		else if (0 == strcmp(stats->field, "fourth_md"))
			cout << team.fourth_md;

		else if (0 == strcmp(stats->field, "fourth_att"))
			cout << team.fourth_att;

		else if (0 == strcmp(stats->field, "fourth_pct"))
			cout << team.fourth_pct;

		else if (0 == strcmp(stats->field, "penalties"))
			cout << team.penalties;

		else if (0 == strcmp(stats->field, "pen_yds"))
			cout << team.pen_yds;

		else if (0 == strcmp(stats->field, "top_per_game"))
			cout << team.top_per_game;

		else if (0 == strcmp(stats->field, "fum"))
			cout << team.fum;

		else if (0 == strcmp(stats->field, "lost"))
			cout << team.lost;

		else if (0 == strcmp(stats->field, "to"))
			cout << team.to;

		else if (0 != strcmp(stats->field, "team_name"))
			cout << "FIELD NOT FOUND: " << stats->field;

		if (stats->years >= 2)
			cout << "\t\t" << (stats->stats + (*index / NO_TEAMS))->year;

		cout << '\n';

		index++;
	}

	cout << '\n';
}

void commands(struct annual_stats *in_stats) {
	int n = 0;
	cin >> n;

	for (int i = 0; i < n; i++) {
		ALL_stats *stats;

		int first;
		int range = 1;
		int constant = 1;
		int o = 0;
		bool x = false;

		char *in_year = new char[IN_LEN];
		char *in_first_year = new char[IN_LEN];
		char *in_last_year = new char[IN_LEN];
		char *command = new char[IN_LEN];
		char *command_2 = new char[IN_LEN];
		char *field = new char[IN_LEN];

		cin >> command;
		cin >> in_year;

		if (0 == strcmp(in_year, "range")) {
			cin >> in_first_year;
			cin >> in_last_year;

			first = atoi(in_first_year);
			range = atoi(in_last_year) - first + 1;
		} else
			first = atoi(in_year);

		cin >> field;
		cin >> command_2;

		if (0 == strcmp(command_2, "decr") || 0 == strcmp(command_2, "max"))
			constant = -1;

		while (!x)
			if ((in_stats + o)->year == first)
				x = true;
			else
				++o;

		if (!x) {
			cout << endl << "YEAR NOT FOUND" << endl;
		} else {
			stats = compare(in_stats + o, range, field, constant);

			if (0 == strcmp(command, "bsort")) {
				bsort(stats);
				out(stats);

			}
			else if (0 == strcmp(command, "qsort")){
				qsort(stats, 0, NO_TEAMS * stats->years - 1);
				out(stats);
			}

			else if (0 == strcmp(command, "bfind")) {
				bsort(stats);

				if (0 == strcmp(command_2, "max")
						|| 0 == strcmp(command_2, "min")) {
					min_max(stats, strcmp(command_2, "max"));

				} else if (0 == strcmp(command_2, "average")) {
					average(stats);

				} else if (0 == strcmp(command_2, "median")) {
					median(stats);

				} else {
					cout << '\n' << "NOT FOUND" << '\n';
				}
			}

			else if (0 == strcmp(command, "qfind")) {
				qsort(stats, 0, NO_TEAMS * stats->years - 1);

				if (0 == strcmp(command_2, "max")
						|| 0 == strcmp(command_2, "min")) {
					min_max(stats, strcmp(command_2, "max"));

				} else if (0 == strcmp(command_2, "average")) {
					average(stats);

				} else if (0 == strcmp(command_2, "median")) {
					median(stats);

				} else {
					cout << '\n' << "NOT FOUND" << '\n';
				}
			}

			delete[] stats->spots;
			delete[] stats->indices;
			delete stats;
		}

		delete[] in_year;
		delete[] in_first_year;
		delete[] in_last_year;
		delete[] command;
		delete[] command_2;
		delete[] field;
	}

	delete[] in_stats;
}

int main() {
	struct annual_stats *stats = in();
	commands(stats);

	return 0;
}
