#include <iostream>
#include "../include/httplib.h"
#include "../include/json.hpp"
#include "../../src/matrix.hpp"

using string = std::string;

using namespace httplib;
using namespace nlohmann;
using namespace matrix;

template<typename T>
void handlePlus(json json, Response &response);

template<typename T>
Matrix<T> fromJson(json json);

template<typename T>
json toJson(const Matrix<T> &mat);

int main() {


    Server svr;

    svr.set_mount_point("/", "webserver/static");

    svr.Get("/", [](const Request &req, Response &resp) {
        resp.set_content("Hello World!", "text/plain");
    });

    svr.Post("/api/plus", [](const Request &req, Response &resp) {
        auto body = json::parse(req.body);
        if (body["type"] == "number") {
            handlePlus<double>(body, resp);
        } else if (body["type"] == "string") {
            handlePlus<string>(body, resp);
        }
    });

    svr.Get("/stop", [&](const Request &req, Response &resp) {
        resp.set_content("stop", "text/plain");
        std::cout << "stop" << std::endl;
        svr.stop();
    });

    std::cout << "start" << std::endl;
    svr.listen("localhost", 8080);

    return 0;
}

template<typename T>
void handlePlus(json json, Response &resp) {
    Matrix<T> mat1 = fromJson<T>(json["mat1"]);
    Matrix<T> mat2 = fromJson<T>(json["mat2"]);
    Matrix<T> sum = mat1 + mat2;
    resp.set_content(toJson(sum).dump(), "application/json");
}


template<typename T>
Matrix<T> fromJson(json json) {
    int rows = json["rows"].get<int>();
    int cols = json["cols"].get<int>();
    Matrix<T> mat(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            mat.unsafe(i, j) = json["data"][i][j];
        }
    }
    return mat;
}

template<typename T>
json toJson(const Matrix<T> &mat) {
    json json;
    json["rows"] = mat.getRows();
    json["cols"] = mat.getCols();
    json["data"] = json::array();
    for (int i = 0; i < mat.getRows(); ++i) {
        json["data"].push_back(json::array());
        for (int j = 0; j < mat.getCols(); ++j) {
            json["data"][i].push_back(mat.unsafe(i, j));
        }
    }
    return json;
}
