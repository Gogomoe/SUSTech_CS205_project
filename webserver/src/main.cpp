#include <iostream>
#include <vector>
#include <stdexcept>
#include <opencv2/highgui/highgui.hpp>
#include "../include/httplib.h"
#include "../include/json.hpp"
#include "../../src/matrix.hpp"

using string = std::string;
using Vec3b = cv::Vec3b;

using namespace httplib;
using namespace nlohmann;
using namespace matrix;

template<typename T>
void handlePlus(json &json, Response &response);

template<typename T>
void handleMinus(json &json, Response &response);

template<typename T>
void handleCrossTimes(json json, Response &resp);

template<typename T>
void handleInnerTimes(json json, Response &resp);

template<typename T>
void handleNegative(json &json, Response &resp);

template<typename T>
void handleSum(json &json, Response &resp);

template<typename T>
void handleSlice(json &json, Response &resp);

template<typename T>
Matrix<T> fromJson(json json);

template<typename T>
json toJson(const Matrix<T> &mat);

Matrix<Vec3b> fromFormData(const MultipartFormData &data);

template<typename T>
Matrix<T> toMatrix(const T &value);

int main() {

    Server svr;

    svr.set_mount_point("/", "webserver/dist");

    svr.Get("/", [](const Request &req, Response &resp) {
        resp.set_content("Hello World!", "text/plain");
    });

    svr.Post("/api/plus", [](const Request &req, Response &resp) {
        auto body = json::parse(req.body);
        if (body["type"] == "number") {
            handlePlus<double>(body, resp);
        } else if (body["type"] == "string") {
            handlePlus<string>(body, resp);
        } else {
            throw std::runtime_error("unsupported plus type");
        }
    });

    svr.Post("/api/image/plus", [](const Request &req, Response &resp) {
        Matrix<Vec3b> img1 = fromFormData(req.get_file_value("img1"));
        Matrix<Vec3b> img2 = fromFormData(req.get_file_value("img2"));
        Matrix<Vec3b> img3 = img1 + img2;
        cv::Mat_<Vec3b> mat = (cv::Mat_<Vec3b>) img3;
        std::vector<uchar> buf;
        cv::imencode(".jpg", mat, buf);
        resp.set_content(string(buf.begin(), buf.end()), "image/jpeg");
    });

    svr.Post("/api/minus", [](const Request &req, Response &resp) {
        auto body = json::parse(req.body);
        if (body["type"] == "number") {
            handleMinus<double>(body, resp);
        } else {
            throw std::runtime_error("unsupported minus type");
        }
    });

    svr.Post("/api/image/minus", [](const Request &req, Response &resp) {
        Matrix<Vec3b> img1 = fromFormData(req.get_file_value("img1"));
        Matrix<Vec3b> img2 = fromFormData(req.get_file_value("img2"));
        Matrix<Vec3b> img3 = img1 - img2;
        cv::Mat_<Vec3b> mat = (cv::Mat_<Vec3b>) img3;
        std::vector<uchar> buf;
        cv::imencode(".jpg", mat, buf);
        resp.set_content(string(buf.begin(), buf.end()), "image/jpeg");
    });

    svr.Post("/api/cross_times", [](const Request &req, Response &resp) {
        auto body = json::parse(req.body);
        if (body["type"] == "number") {
            handleCrossTimes<double>(body, resp);
        } else {
            throw std::runtime_error("unsupported cross_times type");
        }
    });

    svr.Post("/api/inner_times", [](const Request &req, Response &resp) {
        auto body = json::parse(req.body);
        if (body["type"] == "number") {
            handleInnerTimes<double>(body, resp);
        } else {
            throw std::runtime_error("unsupported inner_times type");
        }
    });

    svr.Post("/api/negative", [](const Request &req, Response &resp) {
        auto body = json::parse(req.body);
        if (body["type"] == "number") {
            handleNegative<double>(body, resp);
        } else {
            throw std::runtime_error("unsupported negative type");
        }
    });

    svr.Post("/api/sum", [](const Request &req, Response &resp) {
        auto body = json::parse(req.body);
        if (body["type"] == "number") {
            handleSum<double>(body, resp);
        } else {
            throw std::runtime_error("unsupported sum type");
        }
    });

    svr.Post("/api/slice", [](const Request &req, Response &resp) {
        auto body = json::parse(req.body);
        if (body["type"] == "number") {
            handleSlice<double>(body, resp);
        } else if (body["type"] == "string") {
            handleSlice<string>(body, resp);
        } else {
            throw std::runtime_error("unsupported sum type");
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

Matrix<Vec3b> fromFormData(const MultipartFormData &data) {
    int len = data.content.length();
    const char *buf = data.content.c_str();
    std::vector<char> vec(buf, buf + len);
    cv::Mat_<Vec3b> mat = cv::imdecode(vec, 1);
    return Matrix<Vec3b>(mat);
}

template<typename T>
void handlePlus(json &json, Response &resp) {
    Matrix<T> mat1 = fromJson<T>(json["mat1"]);
    Matrix<T> mat2 = fromJson<T>(json["mat2"]);
    Matrix<T> sum = mat1 + mat2;
    resp.set_content(toJson(sum).dump(), "application/json");
}

template<typename T>
void handleMinus(json &json, Response &resp) {
    Matrix<T> mat1 = fromJson<T>(json["mat1"]);
    Matrix<T> mat2 = fromJson<T>(json["mat2"]);
    Matrix<T> diff = mat1 - mat2;
    resp.set_content(toJson(diff).dump(), "application/json");
}

template<typename T>
void handleCrossTimes(json json, Response &resp) {
    Matrix<T> mat1 = fromJson<T>(json["mat1"]);
    Matrix<T> mat2 = fromJson<T>(json["mat2"]);
    Matrix<T> res = mat1.matmul(mat2);
    resp.set_content(toJson(res).dump(), "application/json");
}

template<typename T>
void handleInnerTimes(json json, Response &resp) {
    Matrix<T> mat1 = fromJson<T>(json["mat1"]);
    Matrix<T> mat2 = fromJson<T>(json["mat2"]);
    Matrix<T> res = mat1.multiply(mat2);
    resp.set_content(toJson(res).dump(), "application/json");
}

template<typename T>
void handleNegative(json &json, Response &resp) {
    Matrix<T> mat1 = fromJson<T>(json["mat1"]);
    resp.set_content(toJson(-mat1).dump(), "application/json");
}

template<typename T>
void handleSum(json &json, Response &resp) {
    Matrix<T> mat1 = fromJson<T>(json["mat1"]);
    resp.set_content(toJson(toMatrix<T>(mat1.sum())).dump(), "application/json");
}

template<typename T>
void handleSlice(json &json, Response &resp) {
    Matrix<T> mat1 = fromJson<T>(json["mat1"]);
    std::vector<int> row = json["row"];
    std::vector<int> col = json["col"];
    resp.set_content(toJson(mat1.slice(row, col)).dump(), "application/json");
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

template<typename T>
Matrix<T> toMatrix(const T &value) {
    Matrix<T> result(1, 1);
    result.unsafe(0, 0) = value;
    return result;
}