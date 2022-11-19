#pragma once

/**
 * @brief Initialize external memory access and access to SRAM
 *
 */
void memory_init();

/**
 * @brief test external SRAM access
 *
 * @note This will fail if compiling with external heap
 *
 */
void memory_test();


/**
 * @brief Add delay between read and write strobes
 * 
 */
void memory_add_delay();