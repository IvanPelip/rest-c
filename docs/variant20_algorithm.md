# Вариант 20 — алгоритм расчёта валютного риска

## Входные данные

- `contract_value_foreign_currency` — стоимость контракта в иностранной валюте, больше `0`;
- `current_fx_rate` — текущий курс, больше `0.0001`;
- `forecasted_volatility_percentage` — прогнозная волатильность, от `0.1` до `200` процентов;
- `hedging_cost_percentage` — стоимость хеджирования, от `0` до `50` процентов.

## Алгоритм

1. Получить четыре значения из JSON-запроса.
2. Убедиться, что все четыре значения переданы и являются конечными числами.
3. Проверить ограничения каждого значения.
4. Если хотя бы одна проверка не пройдена, вернуть HTTP 400 и JSON с описанием ошибки.
5. Рассчитать стоимость контракта в локальной валюте:

   `contract_value_local_currency = contract_value_foreign_currency * current_fx_rate`

6. Рассчитать максимальный валютный риск:

   `maximum_value_at_risk = contract_value_local_currency * forecasted_volatility_percentage / 100`

7. Рассчитать стоимость контракта после затрат на хеджирование:

   `net_value_with_hedging = contract_value_local_currency - contract_value_local_currency * hedging_cost_percentage / 100`

8. Вернуть три результата в JSON и сохранить успешную работу в истории.

## Контрольный пример

Ввод: `10000`, `90`, `10`, `2`.

Вывод: `900000`, `90000`, `882000`.
