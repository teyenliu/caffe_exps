// This program converts a set of images to a lmdb/leveldb by storing them
// as Datum proto buffers.
// Usage:
//   convert_csvset [FLAGS] CSV_FILE_NAME DB_NAME
//

#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <algorithm>
#include <fstream>  // NOLINT(readability/streams)
#include <string>
#include <utility>
#include <iostream>
#include <vector>
#include <array>

#include "boost/scoped_ptr.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/asio.hpp"
#include "gflags/gflags.h"
#include "glog/logging.h"

#include "caffe/proto/caffe.pb.h"
#include "caffe/util/db.hpp"
#include "caffe/util/format.hpp"
#include "caffe/util/io.hpp"
#include "caffe/util/rng.hpp"

using namespace caffe;
using namespace std;
using std::pair;
using boost::scoped_ptr;
using boost::algorithm::trim;

DEFINE_bool(shuffle, false,
		"Randomly shuffle the order of images and their labels");
DEFINE_string(backend, "lmdb",
		"The backend {lmdb, leveldb} for storing the result");

#define DATA_COL_NUM 4
#define LABEL_COL_NUM 2

inline void ConcatenateVectorStrsToStr(vector<pair<string, string>> &ret_lines,
		vector<string> &strs, array<int, DATA_COL_NUM> &data_arr,
		array<int, LABEL_COL_NUM> &label_arr) {

	std::string data;
	std::string label;

	for (int i = 0; i < data_arr.size(); i++) {
		trim(strs[data_arr[i]]);
		if (i < data_arr.size() - 1) {
			data += strs[data_arr[i]] + ",";
		} else {
			data += strs[data_arr[i]];
		}
	}

	for (int i = 0; i < label_arr.size(); i++) {
		trim(strs[label_arr[i]]);
		if (i < label_arr.size() - 1) {
			label += strs[label_arr[i]] + ",";
		} else {
			label += strs[label_arr[i]];
		}
	}

	ret_lines.push_back(std::make_pair(data, label));

	return;
}

int main(int argc, char** argv) {

	std::ifstream infile(argv[2]);
	std::vector<std::pair<std::string, std::string>> lines;
	std::string line;
	size_t pos;

	/*
	 * In this case, we assume label data are in index 4 and 5.
	 */
	while (std::getline(infile, line, '\n')) {
		std::vector < string > strs;

		// trim string from both sides
		trim(line);
		boost::split(strs, line, boost::is_any_of(","));

		array<int, DATA_COL_NUM> data_arr = { 0, 1, 2, 3 };
		array<int, LABEL_COL_NUM> label_arr = { 4, 5 };

		// concatenate data and label string
		ConcatenateVectorStrsToStr(lines, strs, data_arr, label_arr);
	}

	if (FLAGS_shuffle) {
		// randomly shuffle data
		LOG(INFO) << "Shuffling data";
		shuffle(lines.begin(), lines.end());
	}
	LOG(INFO) << "A total of " << lines.size() << " CSV file.";

	// Create new DB
	scoped_ptr<db::DB> db(db::GetDB (FLAGS_backend));
	db->Open(argv[3], db::NEW);
	scoped_ptr < db::Transaction > txn(db->NewTransaction());

	// Storing to db
	int count = 0;
	for (int line_id = 0; line_id < lines.size(); ++line_id) {

		// sequential
		string key_str = caffe::format_int(line_id, 8) + "_"
				+ lines[line_id].first;

		// Put in db
		string out = lines[line_id].first + ":" + lines[line_id].second;
		std::cout << "print key_str and out:" << key_str << "  " << out
				<< std::endl;
		txn->Put(key_str, out);

		if (++count % 1000 == 0) {
			// Commit db
			txn->Commit();
			txn.reset(db->NewTransaction());
			LOG(INFO) << "Processed " << count << " files.";
		}
	}
	// write the last batch
	if (count % 1000 != 0) {
		txn->Commit();
		LOG(INFO) << "Processed " << count << " files.";
	}
	return 0;
}
