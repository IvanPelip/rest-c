# Тестирование варианта 20

Маршрут: `POST /api/v1.1/works/calc-individual`.

Перед каждым запросом требуется Bearer-токен пользователя с ролью `user`, `admin` или `owner`.

| № | Данные | Что проверяется | Ожидаемый результат |
|---:|---|---|---|
| 1 | contract `10000`, rate `90`, volatility `10`, hedging `2` | Контрольный пример | HTTP 200; local `900000`, risk `90000`, net `882000` |
| 2 | contract `10000`, rate `0.0002`, volatility `0.1`, hedging `0` | Нижние допустимые значения курса, волатильности и стоимости хеджирования | HTTP 200; local `2`, risk `0.002`, net `2` |
| 3 | contract `10000`, rate `90`, volatility `200`, hedging `50` | Верхние границы волатильности и хеджирования | HTTP 200; local `900000`, risk `1800000`, net `450000` |
| 4 | contract `0`, rate `90`, volatility `10`, hedging `2` | Недопустимая стоимость контракта | HTTP 400; `contract_value_foreign_currency must be > 0` |
| 5 | contract `10000`, rate `0.0001`, volatility `10`, hedging `2` | Граница курса, которая не входит в диапазон | HTTP 400; `current_fx_rate must be > 0.0001` |
| 6 | contract `10000`, rate `90`, volatility `0`, hedging `2` | Волатильность ниже допустимой | HTTP 400; `forecasted_volatility_percentage must be in [0.1, 200.0]` |
| 7 | contract `10000`, rate `90`, volatility `10`, hedging `50.1` | Хеджирование выше допустимого | HTTP 400; `hedging_cost_percentage must be in [0.0, 50.0]` |
| 8 | Не передано одно из четырёх полей | Неполный JSON | HTTP 400; `All four fields must be finite JSON numbers` |

## JSON для ручного запуска

### Верхняя граница

```json
{
  "contract_value_foreign_currency": 10000,
  "current_fx_rate": 90,
  "forecasted_volatility_percentage": 200,
  "hedging_cost_percentage": 50
}
```

### Ошибка: курс ниже допустимого

```json
{
  "contract_value_foreign_currency": 10000,
  "current_fx_rate": 0.0001,
  "forecasted_volatility_percentage": 10,
  "hedging_cost_percentage": 2
}
```
