#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <string>

#include "ppm.h"

// lttng-ust trace points
#define TRACEPOINT_DEFINE
#define TRACEPOINT_PROBE_DYNAMIC_LINKAGE
#include "tp_ppm.h"

//init with default values
void ppm::init() {
    width = 0;
    height = 0;
    max_col_val = 255;
}

//create a PPM object
ppm::ppm() {
    init();
    tracepoint(com_vanwinkeljan_lttng_test_ppm, create,width,height,"");
}

//create a PPM object and fill it with data stored in fname 
ppm::ppm(const std::string &fname) {
    init();
    read(fname);
    std::stringstream tmpStrStream;
    tmpStrStream << "Created PPM class and read data from file " << fname;
    tracepoint(com_vanwinkeljan_lttng_test_ppm, create, width,height,fname.c_str());
}

//create an "epmty" PPM image with a given width and height;the R,G,B arrays are filled with zeros
ppm::ppm(const unsigned int _width, const unsigned int _height) {
    init();
    width = _width;
    height = _height;
    nr_lines = height;
    nr_columns = width;
    size = width*height;

    buff.resize(3*size);

    // fill r, g and b with 0
    r.resize(size);
    g.resize(size);
    b.resize(size);

    tracepoint(com_vanwinkeljan_lttng_test_ppm, create, width,height,"");
}

//read the PPM image from fname
void ppm::read(const std::string &fname) {
    std::ifstream inp(fname.c_str(), std::ios::in | std::ios::binary);
    if (inp.is_open()) {
        std::string line;
        std::getline(inp, line);
        if (line != "P6") {
        	std::stringstream msg;
        	msg << "Unrecognized file format (file: " << fname << ")";
        	tracepoint(com_vanwinkeljan_lttng_test_ppm,
        			error_message,
        			msg.str().c_str());
            return;
        }
        std::getline(inp, line);
        while (line[0] == '#') {
            std::getline(inp, line);
        }
        std::stringstream dimensions(line);

        try {
            dimensions >> width;
            dimensions >> height;
            nr_lines = height;
            nr_columns = width;
        } catch (std::exception &e) {
        	std::stringstream msg;
        	msg << "Header file format error (file: " << fname << ")" << e.what();
        	tracepoint(com_vanwinkeljan_lttng_test_ppm,
        			error_message,
        			msg.str().c_str());
        	return;
        }

        std::getline(inp, line);
        std::stringstream max_val(line);
        try {
            max_val >> max_col_val;
        } catch (std::exception &e) {
        	std::stringstream msg;
        	msg << "Header file format error (file: " << fname << ")" << e.what();
        	tracepoint(com_vanwinkeljan_lttng_test_ppm,
        			error_message,
        			msg.str().c_str());
            return;
        }

        size = width*height;

        r.reserve(size);
        g.reserve(size);
        b.reserve(size);

        char aux;
        for (unsigned int i = 0; i < size; ++i) {
            inp.read(&aux, 1);
            r[i] = (unsigned char) aux;
            inp.read(&aux, 1);
            g[i] = (unsigned char) aux;
            inp.read(&aux, 1);
            b[i] = (unsigned char) aux;
        }
    } else {
    	std::stringstream msg;
    	msg << "Error. Unable to open " << fname;
    	tracepoint(com_vanwinkeljan_lttng_test_ppm,
    			error_message,
    			msg.str().c_str());
    }
    inp.close();
    tracepoint(com_vanwinkeljan_lttng_test_ppm, read,fname.c_str());
}

//write the PPM image in fname
void ppm::write(const std::string &fname) {
    std::ofstream inp(fname.c_str(), std::ios::out | std::ios::binary);
    if (inp.is_open()) {

        inp << "P6\n";
        inp << width;
        inp << " ";
        inp << height << "\n";
        inp << max_col_val << "\n";

        unsigned int ps = 0;
        for (unsigned int i = 0; i < size; ++i) {
            buff[ps] = (char) r[i]; ps++;
            buff[ps] = (char) g[i]; ps++;
            buff[ps] = (char) b[i]; ps++;
        }
        inp.write(&buff[0], 3*size);
    } else {
    	std::stringstream msg;
    	msg << "Error. Unable to open " << fname;
    	tracepoint(com_vanwinkeljan_lttng_test_ppm,
    			error_message,
    			msg.str().c_str());
    }
    inp.close();
    tracepoint(com_vanwinkeljan_lttng_test_ppm, write,fname.c_str());
}
