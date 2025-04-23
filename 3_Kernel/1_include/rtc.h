
#include <stdint.h>

#ifndef TPE_ARQUI_RTC_H
#define TPE_ARQUI_RTC_H

#define RTC_SECONDS         0x00
#define RTC_SECONDS_ALARM   0x01
#define RTC_MINUTES         0x02
#define RTC_MINUTES_ALARM   0x03
#define RTC_HOURS           0x04
#define RTC_HOURS_ALARM     0x05
#define RTC_DAY_OF_WEEK     0x06
#define RTC_DAY_OF_MONTH    0x07
#define RTC_MONTH           0x08
#define RTC_YEAR            0x09

/**
 * @brief Retrieves the current seconds from the Real-Time Clock (RTC).
 * @return The current seconds value (0-59).
 */
uint8_t getRTCSeconds();

/**
 * @brief Retrieves the current minutes from the Real-Time Clock (RTC).
 * @return The current minutes value (0-59).
 */
uint8_t getRTCMinutes();

/**
 * @brief Retrieves the current hours from the Real-Time Clock (RTC).
 * @return The current hours value (0-23).
 */
uint8_t getRTCHours();

/**
 * @brief Retrieves the current day of the week from the Real-Time Clock (RTC).
 * @return The current day of the week value (1-7, where 1 = Sunday).
 */
uint8_t getRTCDayOfWeek();

/**
 * @brief Retrieves the current day of the month from the Real-Time Clock (RTC).
 * @return The current day of the month value (1-31).
 */
uint8_t getRTCDayOfMonth();

/**
 * @brief Retrieves the current month from the Real-Time Clock (RTC).
 * @return The current month value (1-12).
 */
uint8_t getRTCMonth();

/**
 * @brief Retrieves the current year from the Real-Time Clock (RTC).
 * @return The current year value (e.g., 2023).
 */
uint8_t getRTCYear();

#endif //TPE_ARQUI_RTC_H