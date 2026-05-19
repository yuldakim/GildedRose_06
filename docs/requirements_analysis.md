# Gilded Rose 요구사항 분석

## 1. 아이템 타입별 비즈니스 규칙

| 아이템 타입 | 식별 문자열 | `sellIn` 변화 | `quality` 변화 | 경계/예외 |
|---|---|---:|---|---|
| Normal | 그 외 일반 아이템 | 매일 -1 | 기본 -1, 판매 기한 경과 후 -2 | `quality`는 0 미만 불가 |
| Aged Brie | `Aged Brie` | 매일 -1 | 기본 +1, 판매 기한 경과 후 +2 | `quality`는 50 초과 불가 |
| Backstage Pass | `Backstage passes to a TAFKAL80ETC concert` | 매일 -1 | `sellIn > 10`: +1, `10 >= sellIn > 5`: +2, `5 >= sellIn >= 0`: +3 | 콘서트 이후 `quality = 0`, 50 초과 불가 |
| Sulfuras | `Sulfuras, Hand of Ragnaros` | 변화 없음 | 변화 없음 | 전설 아이템으로 `quality = 80` 허용, 값 변경 금지 |
| Conjured | `Conjured` 계열 아이템 | 매일 -1 | Normal보다 2배 빠르게 감소. 기본 -2, 판매 기한 경과 후 -4 | `quality`는 0 미만 불가 |

## 2. 문자열 비교/분기 시 주의점

1. 현재 C++ 구현은 `std::string`의 정확한 문자열 비교(`==`, `!=`)에 강하게 의존한다.
2. `Aged Brie`, `Sulfuras`, `Backstage Pass`는 요구사항상 고정 이름이므로 `==` 비교가 가장 명확하다.
3. 문자열 리터럴은 오탈자에 취약하므로 구현에서는 이름 있는 상수로 분리하는 것이 좋다.
4. `Conjured`는 신규 카테고리이므로 식별 정책을 먼저 확정해야 한다.
5. `Conjured` 아이템을 정확히 이름이 `Conjured`인 항목만으로 볼지, `Conjured Mana Cake`처럼 `Conjured`로 시작하는 모든 항목으로 볼지 테스트에서 명시해야 한다.
6. `std::string::find()`는 찾은 위치를 반환하며, 찾지 못하면 `std::string::npos`를 반환한다.
7. 다음 코드는 위험하다.

```cpp
if (name.find("Conjured")) {
    // 잘못된 분기 가능
}
```

8. 위 코드는 `"Conjured"`가 문자열의 시작 위치에 있으면 `0`이 반환되어 false처럼 처리되고, 찾지 못하면 `npos`가 반환되어 true처럼 처리될 수 있다.
9. `find()`를 사용할 때는 반드시 `std::string::npos`와 비교해야 한다.

```cpp
if (name.find("Conjured") != std::string::npos) {
    // 포함 여부 확인
}
```

10. 접두어 기준이 필요하다면 C++17에는 `std::string::starts_with()`가 없으므로 `name.rfind("Conjured", 0) == 0` 같은 명시적인 조건을 사용한다.

## 3. 예외/경계값 조건

1. `quality`는 `Sulfuras`를 제외하고 항상 0 이상 50 이하를 유지해야 한다.
2. `quality = 0`인 감소형 아이템은 더 이상 감소하지 않아야 한다.
3. `quality = 50`인 증가형 아이템은 더 이상 증가하지 않아야 한다.
4. `sellIn = 0`은 업데이트 시점에 아직 현재 날짜로 취급되며, 업데이트 후 `sellIn = -1`이 되어 만료 상태가 된다.
5. `sellIn = -1` 및 그보다 작은 음수는 허용된다. `sellIn`을 0으로 보정하거나 음수를 막으면 안 된다.
6. Normal 아이템은 판매 기한 경과 후 하루에 `quality`가 2 감소한다.
7. `Aged Brie`는 판매 기한 경과 후 하루에 `quality`가 2 증가한다.
8. `Backstage Pass`는 콘서트일이 지나면 `quality`가 0이 된다.
9. `Sulfuras`는 `sellIn`과 `quality`가 모두 절대 변경되지 않아야 한다.
10. `Sulfuras`의 `quality = 80`은 일반 상한 50의 예외이며, 50으로 보정하면 안 된다.

## 4. Conjured 신규 요구사항

1. `Conjured` 아이템은 일반 아이템보다 `quality`가 2배 빠르게 감소한다.
2. 판매 기한 전에는 하루에 `quality`가 2 감소한다.
3. 판매 기한이 지난 후에는 일반 아이템의 만료 후 감소량도 2배가 되어 하루에 `quality`가 4 감소한다.
4. `quality`는 감소량이 크더라도 0 미만으로 내려가면 안 된다.
5. `Conjured`는 `Sulfuras`, `Aged Brie`, `Backstage Pass`와 독립된 아이템 타입으로 분기해야 한다.
6. 권장 식별 정책은 이름이 `Conjured`로 시작하는 아이템을 `Conjured` 카테고리로 처리하는 것이다.
7. 이 정책을 사용할 경우 `Conjured Mana Cake`, `Conjured Sword` 같은 이름도 동일한 감소율을 적용한다.

## 5. Google Test 기준 테스트 시나리오 목록

1. Normal 아이템은 하루 후 `sellIn`이 1 감소하고 `quality`가 1 감소한다.
2. Normal 아이템은 `sellIn = 0`에서 업데이트 후 `sellIn = -1`, `quality`가 2 감소한다.
3. Normal 아이템은 `sellIn = -1`에서 업데이트 후 `sellIn = -2`, `quality`가 2 감소한다.
4. Normal 아이템의 `quality = 0`은 감소하지 않는다.
5. Normal 아이템의 `quality = 1`, `sellIn = 0`은 업데이트 후 `quality = 0`으로 하한을 지킨다.
6. `Aged Brie`는 하루 후 `sellIn`이 1 감소하고 `quality`가 1 증가한다.
7. `Aged Brie`는 `sellIn = 0`에서 업데이트 후 `sellIn = -1`, `quality`가 2 증가한다.
8. `Aged Brie`의 `quality = 50`은 증가하지 않는다.
9. `Aged Brie`의 `quality = 49`, `sellIn = 0`은 업데이트 후 `quality = 50`으로 상한을 지킨다.
10. `Backstage Pass`는 `sellIn > 10`일 때 `quality`가 1 증가한다.
11. `Backstage Pass`는 `sellIn = 10`일 때 `quality`가 2 증가한다.
12. `Backstage Pass`는 `sellIn = 5`일 때 `quality`가 3 증가한다.
13. `Backstage Pass`는 `sellIn = 0`에서 업데이트 후 `sellIn = -1`, `quality = 0`이 된다.
14. `Backstage Pass`는 증가하더라도 `quality = 50`을 초과하지 않는다.
15. `Sulfuras`는 업데이트 후에도 `sellIn`과 `quality`가 모두 변하지 않는다.
16. `Sulfuras`의 `quality = 80`은 50으로 보정되지 않는다.
17. `Conjured` 아이템은 판매 기한 전 하루에 `quality`가 2 감소한다.
18. `Conjured` 아이템은 `sellIn = 0`에서 업데이트 후 `sellIn = -1`, `quality`가 4 감소한다.
19. `Conjured` 아이템은 `sellIn = -1`에서 업데이트 후 `sellIn = -2`, `quality`가 4 감소한다.
20. `Conjured` 아이템의 `quality`는 0 미만으로 내려가지 않는다.
21. `Conjured` 접두어 정책을 채택한다면 `Conjured Mana Cake`도 `Conjured` 규칙을 적용받는다.
22. 여러 아이템이 함께 있을 때 각 아이템 타입별 규칙이 독립적으로 적용된다.
