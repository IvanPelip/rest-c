#pragma once
// ============================================================================
// Вариант 20. Оценщик валютных рисков при операциях.
// ============================================================================

#include <cerrno>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <string>
#include "../../json.hpp"

namespace TaskVariant {

    struct Input {
        double contract_value_foreign_currency = 0.0;
        double current_fx_rate = 0.0;
        double forecasted_volatility_percentage = 0.0;
        double hedging_cost_percentage = 0.0;
        std::string parse_error;
    };

    struct Output {
        double contract_value_local_currency;
        double maximum_value_at_risk;
        double net_value_with_hedging;
    };

    // Читает JSON-число и не позволяет пропущенному полю маскироваться под 0.
    inline bool read_number(const std::string& body, const std::string& key,
                            double& value) {
        const std::string quoted_key = "\"" + key + "\"";
        size_t pos = body.find(quoted_key);
        if (pos == std::string::npos) return false;
        pos = body.find(':', pos + quoted_key.size());
        if (pos == std::string::npos) return false;
        ++pos;
        while (pos < body.size() && std::isspace(static_cast<unsigned char>(body[pos]))) ++pos;

        errno = 0;
        const char* begin = body.c_str() + pos;
        char* end = nullptr;
        value = std::strtod(begin, &end);
        if (begin == end || errno == ERANGE || !std::isfinite(value)) return false;
        while (*end && std::isspace(static_cast<unsigned char>(*end))) ++end;
        return *end == ',' || *end == '}';
    }

    inline Input parse(const std::string& body) {
        Input in;
        if (!read_number(body, "contract_value_foreign_currency", in.contract_value_foreign_currency) ||
            !read_number(body, "current_fx_rate", in.current_fx_rate) ||
            !read_number(body, "forecasted_volatility_percentage", in.forecasted_volatility_percentage) ||
            !read_number(body, "hedging_cost_percentage", in.hedging_cost_percentage)) {
            in.parse_error = "All four fields must be finite JSON numbers";
        }
        return in;
    }

    inline std::string validate(const Input& in) {
        if (!in.parse_error.empty()) return in.parse_error;
        if (in.contract_value_foreign_currency <= 0.0)
            return "contract_value_foreign_currency must be > 0.0";
        if (in.current_fx_rate <= 0.0001)
            return "current_fx_rate must be > 0.0001";
        if (in.forecasted_volatility_percentage < 0.1 ||
            in.forecasted_volatility_percentage > 200.0)
            return "forecasted_volatility_percentage must be in [0.1, 200.0]";
        if (in.hedging_cost_percentage < 0.0 || in.hedging_cost_percentage > 50.0)
            return "hedging_cost_percentage must be in [0.0, 50.0]";
        return "";
    }

    inline Output calculate(const Input& in) {
        Output out{};
        out.contract_value_local_currency =
            in.contract_value_foreign_currency * in.current_fx_rate;
        out.maximum_value_at_risk = out.contract_value_local_currency *
            (in.forecasted_volatility_percentage / 100.0);
        out.net_value_with_hedging = out.contract_value_local_currency -
            out.contract_value_local_currency * (in.hedging_cost_percentage / 100.0);
        return out;
    }

    inline std::string to_json(const Output& out) {
        JObj o;
        o.num("contract_value_local_currency", out.contract_value_local_currency);
        o.num("maximum_value_at_risk", out.maximum_value_at_risk);
        o.num("net_value_with_hedging", out.net_value_with_hedging);
        return o.str();
    }
}
