/*!
 * \file labsat23_source.h
 *
 * \brief Unpacks the Labsat 2 (ls2) and (ls3) capture files
 * \author Javier Arribas jarribas (at) cttc.es
 * -------------------------------------------------------------------------
 *
 * Copyright (C) 2010-2019  (see AUTHORS file for a list of contributors)
 *
 * GNSS-SDR is a software defined Global Navigation
 *          Satellite Systems receiver
 *
 * This file is part of GNSS-SDR.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * -------------------------------------------------------------------------
 */

#ifndef GNSS_SDR_LABSAT23_SOURCE_H
#define GNSS_SDR_LABSAT23_SOURCE_H

#include "concurrent_queue.h"
#include <gnuradio/block.h>
#include <pmt/pmt.h>
#include <cstdint>
#include <fstream>
#include <memory>
#include <string>


class labsat23_source;

using labsat23_source_sptr = boost::shared_ptr<labsat23_source>;

labsat23_source_sptr labsat23_make_source_sptr(
    const char *signal_file_basename,
    int channel_selector,
    std::shared_ptr<Concurrent_Queue<pmt::pmt_t>> queue);

/*!
 * \brief This class implements conversion between Labsat2 and 3 format byte packet samples to gr_complex
 */
class labsat23_source : public gr::block
{
public:
    ~labsat23_source();

    int general_work(int noutput_items,
        gr_vector_int &ninput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items);

private:
    friend labsat23_source_sptr labsat23_make_source_sptr(
        const char *signal_file_basename,
        int channel_selector,
        std::shared_ptr<Concurrent_Queue<pmt::pmt_t>> queue);

    labsat23_source(const char *signal_file_basename,
        int channel_selector,
        std::shared_ptr<Concurrent_Queue<pmt::pmt_t>> queue);

    std::string generate_filename();
    void decode_samples_one_channel(int16_t input_short, gr_complex *out, int type);
    int getBit(uint8_t byte, int position);
    bool d_header_parsed;
    uint8_t d_channel_selector;
    int d_channel_selector_config;
    int d_current_file_number;
    uint8_t d_labsat_version;
    std::string d_signal_file_basename;
    std::ifstream *binary_input_file;
    uint8_t d_ref_clock;
    uint8_t d_bits_per_sample;
    std::shared_ptr<Concurrent_Queue<pmt::pmt_t>> d_queue;
};

#endif  // GNSS_SDR_LABSAT23_SOURCE_H
