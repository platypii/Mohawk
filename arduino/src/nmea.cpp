#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mohawk.h"

#define parseInt2(a, b) ((a - '0') * 10 + (b - '0'))
#define KNOTS 0.51444444444444444

static void parse_gga(const char *line);
static void parse_rmc(char *line);
static double parse_degrees_minutes(const char *dm, const char *nsew);
static double parse_double(const char *str);

// Global altitude
static double altitude = NAN;

void parse_nmea(char *line) {
  if (strnlen(line, 255) < 7) {
    return;
  }
  if (strncmp(line + 3, "RMC,", 4) == 0) {
    parse_rmc(line);
  } else if (strncmp(line + 3, "GGA,", 4) == 0) {
    parse_gga(line);
  }
}

/**
 * Parse GPS altitude from GGA sentence
 */
static void parse_gga(const char *line) {
  // Parse altitude from split[9]
  int commas = 0;
  for (int i = 0; line[i]; i++) {
    if (line[i] == ',') {
      if (++commas == 9) {
        altitude = parse_double(line + i + 1);
      }
    }
  }
}

/**
 * Parse location from RMC sentence
 */
static void parse_rmc(char *line) {
  // char time_str[10], status, latDM[11], latNS, lngDM[11], lngEW, date_str[7];
  const char *TOK = ",";
  strsep(&line, TOK); // command
  char *time_str = strsep(&line, TOK);
  strsep(&line, TOK); // status
  char *latDM = strsep(&line, TOK);
  char *latNS = strsep(&line, TOK);
  char *lngDM = strsep(&line, TOK);
  char *lngEW = strsep(&line, TOK);
  char *hspeed_str = strsep(&line, TOK);
  char *bearing_str = strsep(&line, TOK);
  char *date_str = strsep(&line, TOK);

  if (!date_str || !time_str) {
    return;
  }

  struct tm date;
  // Parse HHMMSS.SS time
  date.tm_hour = parseInt2(time_str[0], time_str[1]);
  date.tm_min = parseInt2(time_str[2], time_str[3]);
  date.tm_sec = parseInt2(time_str[4], time_str[5]);
  // Parse DDMMYY date
  date.tm_mday = parseInt2(date_str[0], date_str[1]);
  date.tm_mon = parseInt2(date_str[2], date_str[3]) - 1;
  date.tm_year = parseInt2(date_str[4], date_str[5]) + 100;
  const short milli = parseInt2(time_str[7], time_str[8]) * 10;

  const double lat = parse_degrees_minutes(latDM, latNS);
  const double lng = parse_degrees_minutes(lngDM, lngEW);
  const double climb = NAN; // TODO: Kalman
  const double hspeed = parse_double(hspeed_str) * KNOTS;
  double bearing = parse_double(bearing_str);
  if (isnan(bearing)) {
    // At low speeds, bearing is often NaN
    // Setting it to north is weird but better than throwing away the point
    bearing = 0;
  }
  const double vN = hspeed * cos(to_radians(bearing));
  const double vE = hspeed * sin(to_radians(bearing));

  // Filter out unlikely lat/lng
  if (fabs(lat) > 0.1 || fabs(lng) > 0.1) {
    // Generate point
    GeoPointV *point = new GeoPointV { date, milli, lat, lng, altitude, climb, vN, vE };
    update_location(point);
  }
}

/**
 * Parse DDDMM.MMMM,N into decimal degrees
 * @param dm The latitude or longitude in "DDDMM.MMMM" format
 * @param nsew The modifier "N", "S", "E", or "W"
 * @return The latitude or longitude in decimal degrees
 */
static double parse_degrees_minutes(const char *dm, const char *nsew) {
  if (!*dm || !*nsew) {
    return NAN;
  }
  int index = strchr(dm, '.') - dm - 2;
  char degrees_str[4] = "";
  strncpy(degrees_str, dm, index);
  double d = atof(degrees_str);
  double m = atof(dm + index);
  double degrees = d + m / 60.0;
  if (*nsew == 'S' || *nsew == 'W') {
    return -degrees;
  } else {
    return degrees;
  }
}

/**
 * Parse double from string, nan on parse error.
 */
static double parse_double(const char *str) {
  char *end;
  double parsed = strtod(str, &end);
  if (str != end) {
    return parsed;
  } else {
    return NAN;
  }
}
