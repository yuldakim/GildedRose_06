# Gilded Rose 테스트 계획서

## 1. 목적 및 범위

본 문서는 C++17 기반 Gilded Rose 구현의 비즈니스 규칙을 검증하기 위한 Google Test 단위 테스트 계획을 정의한다. 주요 검증 대상은 `GildedRose::updateQuality()`이며, 요구사항상 변경 금지 대상인 `Item` 클래스와 `GildedRose::items` 공개 속성은 구조 변경 없이 테스트 관찰 지점으로만 사용한다.

테스트는 다음 목표를 가진다.

1. 아이템 타입별 `sellIn`, `quality` 변화 규칙을 명확히 검증한다.
2. `quality` 하한 0, 상한 50, `Sulfuras` 예외 80을 경계값 중심으로 검증한다.
3. 신규 `Conjured` 요구사항을 기존 일반 아이템 규칙과 분리하여 검증한다.
4. 리팩터링 또는 기능 추가 이후에도 기존 동작이 회귀하지 않도록 빠른 피드백을 제공한다.
5. gcov/lcov 기준 라인 및 브랜치 커버리지 90% 이상을 목표로 관리한다.

## 2. 테스트 환경

| 항목 | 기준 |
|---|---|
| 언어 | C++17 |
| 빌드 | CMake |
| 테스트 프레임워크 | Google Test |
| 테스트 실행 | CTest 또는 `gilded_rose_test` 직접 실행 |
| 커버리지 도구 | gcov/lcov, genhtml |
| 주요 테스트 대상 | `cpp/src/GildedRose.cpp`, `cpp/include/GildedRose.h` |

권장 실행 흐름은 다음과 같다.

```bash
cmake -S cpp -B cpp/build -DCMAKE_BUILD_TYPE=Debug
cmake --build cpp/build
ctest --test-dir cpp/build --output-on-failure
```

## 3. 테스트 설계 원칙

1. `TEST_F` 기반 fixture를 사용해 테스트 데이터 생성과 검증 헬퍼를 공통화한다.
2. 테스트명은 규칙과 기대 결과가 드러나도록 `ItemType_WhenCondition_ThenExpectedResult` 형식으로 작성한다.
3. 각 테스트는 가능한 한 하나의 비즈니스 규칙만 검증한다.
4. 경계값 테스트는 `quality`와 `sellIn`의 임계값을 직접 드러내는 입력값을 사용한다.
5. 요구사항이 명시한 특수 아이템 이름은 문자열 상수로 관리해 오탈자 리스크를 줄인다.
6. 다중 아이템 테스트는 개별 규칙 검증 이후, 루프 처리와 아이템 간 독립성을 확인하는 보조 테스트로 둔다.

권장 fixture 구조는 다음과 같다.

```cpp
class GildedRoseTest : public ::testing::Test {
protected:
    std::vector<Item> items;

    void update() {
        GildedRose app(items);
        app.updateQuality();
    }

    Item& addItem(const std::string& name, int sellIn, int quality) {
        items.emplace_back(name, sellIn, quality);
        return items.back();
    }
};
```

## 4. TEST_F 단위 테스트 범위 및 우선순위

### P0: 핵심 규칙 및 회귀 방지

P0 테스트는 모든 구현 변경 전에 먼저 작성되어야 하며, 실패 시 배포 또는 리팩터링을 중단한다.

| 테스트 그룹 | 검증 범위 | 대표 테스트 |
|---|---|---|
| Normal item | 일반 아이템의 기본 감소 및 만료 후 2배 감소 | `NormalItem_WhenUpdatedBeforeSellDate_DecreasesSellInAndQualityByOne` |
| Quality lower bound | 감소형 아이템의 `quality >= 0` 불변식 | `NormalItem_WhenQualityIsZero_DoesNotGoNegative` |
| Aged Brie | 시간이 지날수록 증가, 만료 후 2배 증가 | `AgedBrie_WhenSellDatePassed_IncreasesQualityByTwo` |
| Quality upper bound | 증가형 아이템의 `quality <= 50` 불변식 | `AgedBrie_WhenQualityIsFifty_DoesNotIncrease` |
| Backstage passes | 10일, 5일 임계 구간별 증가량 및 콘서트 후 0 | `BackstagePass_WhenSellInIsFive_IncreasesQualityByThree` |
| Sulfuras | `sellIn`, `quality` 모두 불변 | `Sulfuras_WhenUpdated_DoesNotChangeSellInOrQuality` |
| Conjured | 일반 아이템보다 2배 빠른 감소 | `ConjuredItem_WhenUpdatedBeforeSellDate_DecreasesQualityByTwo` |

### P1: 경계값과 조합 케이스

P1 테스트는 비즈니스 규칙의 임계값을 촘촘히 검증하고, 리팩터링 중 조건문 오류를 조기에 발견하는 데 집중한다.

| 테스트 그룹 | 검증 범위 | 대표 테스트 |
|---|---|---|
| `sellIn = 0` | 업데이트 후 만료 상태가 되는 날의 처리 | `NormalItem_WhenSellInIsZero_DecreasesQualityByTwo` |
| `sellIn = -1` | 이미 만료된 아이템 처리 | `NormalItem_WhenSellInIsNegative_DecreasesQualityByTwo` |
| `quality = 1` | 감소량이 남은 품질보다 큰 경우 하한 보정 | `ConjuredItem_WhenQualityIsOne_DoesNotGoNegative` |
| `quality = 49` | 증가량이 상한을 넘는 경우 50으로 제한 | `AgedBrie_WhenQualityIsFortyNineAndExpired_CapsAtFifty` |
| `quality = 50` | 증가형 아이템 상한 유지 | `BackstagePass_WhenQualityIsFifty_DoesNotExceedFifty` |
| 다중 아이템 | 한 번의 업데이트에서 각 아이템 독립 처리 | `MultipleItems_WhenUpdated_AppliesEachRuleIndependently` |

### P2: 명세 해석 및 유지보수 리스크

P2 테스트는 요구사항 해석 차이 또는 문자열 분기 오류를 방지하기 위한 보강 테스트다.

| 테스트 그룹 | 검증 범위 | 대표 테스트 |
|---|---|---|
| Conjured 식별 정책 | `Conjured` 접두어 이름 처리 여부 | `ConjuredItem_WhenNameStartsWithConjured_UsesConjuredRule` |
| 알 수 없는 이름 | 특수 이름이 아닌 모든 항목은 Normal 처리 | `UnknownItem_WhenUpdated_UsesNormalRule` |
| 빈 컬렉션 | 아이템이 없어도 안전하게 종료 | `NoItems_WhenUpdated_DoesNothing` |
| 긴 기간 시뮬레이션 | 여러 날 업데이트 후 불변식 유지 | `Items_WhenUpdatedForManyDays_MaintainQualityBounds` |

## 5. 경계값 케이스 목록

### Quality 경계값

| `quality` | Normal | Aged Brie | Backstage Pass | Sulfuras | Conjured |
|---:|---|---|---|---|---|
| 0 | 감소하지 않음 | 1 또는 2로 증가 가능 | sellIn 구간에 따라 증가 또는 만료 후 0 | 변경 없음 | 감소하지 않음 |
| 1 | 0으로 감소, 음수 금지 | 증가 | 증가 또는 만료 후 0 | 변경 없음 | 0으로 감소, 음수 금지 |
| 49 | 감소 | 최대 50까지만 증가 | 최대 50까지만 증가 | 일반 상한 대상 아님 | 감소 |
| 50 | 감소 가능 | 50 유지 | 50 유지 또는 만료 후 0 | 일반 상한 대상 아님 | 감소 |

필수 테스트 케이스:

1. Normal, `quality = 0`, `sellIn > 0`: 업데이트 후 `quality = 0`.
2. Normal, `quality = 1`, `sellIn = 0`: 업데이트 후 `quality = 0`.
3. Aged Brie, `quality = 49`, `sellIn = 0`: 업데이트 후 `quality = 50`.
4. Aged Brie, `quality = 50`, `sellIn >= 0`: 업데이트 후 `quality = 50`.
5. Backstage Pass, `quality = 49`, `sellIn = 10`: 업데이트 후 `quality = 50`.
6. Backstage Pass, `quality = 49`, `sellIn = 5`: 업데이트 후 `quality = 50`.
7. Backstage Pass, `quality = 50`, `sellIn = 10`: 업데이트 후 `quality = 50`.
8. Backstage Pass, `quality = 50`, `sellIn = 0`: 업데이트 후 `quality = 0`.
9. Sulfuras, `quality = 80`: 업데이트 후 `quality = 80`.
10. Conjured, `quality = 1`, `sellIn > 0`: 업데이트 후 `quality = 0`.
11. Conjured, `quality = 1`, `sellIn = 0`: 업데이트 후 `quality = 0`.

### SellIn 경계값

| `sellIn` | 의미 | Normal | Aged Brie | Backstage Pass | Sulfuras | Conjured |
|---:|---|---|---|---|---|---|
| 0 | 판매 마지막 날, 업데이트 후 만료 | `quality -2`, `sellIn -1` | `quality +2`, `sellIn -1` | `quality = 0`, `sellIn -1` | 변경 없음 | `quality -4`, `sellIn -1` |
| -1 | 이미 만료됨 | `quality -2`, `sellIn -1` | `quality +2`, `sellIn -1` | `quality = 0`, `sellIn -1` | 변경 없음 | `quality -4`, `sellIn -1` |

추가로 Backstage Pass 임계값은 다음 값을 반드시 포함한다.

1. `sellIn = 11`: `quality +1`.
2. `sellIn = 10`: `quality +2`.
3. `sellIn = 6`: `quality +2`.
4. `sellIn = 5`: `quality +3`.
5. `sellIn = 1`: `quality +3`.
6. `sellIn = 0`: 업데이트 후 콘서트 이후로 간주되어 `quality = 0`.

## 6. 예외 및 특이 케이스 목록

1. `Sulfuras`는 `sellIn`도 감소하지 않고 `quality`도 변경되지 않는다.
2. `Sulfuras`의 `quality = 80`은 유효한 예외값이며 50으로 보정하면 안 된다.
3. `quality`는 `Sulfuras`를 제외하고 0 미만으로 내려가면 안 된다.
4. `quality`는 `Sulfuras`를 제외하고 50을 초과하면 안 된다.
5. Backstage Pass는 `sellIn = 0`에서 업데이트 후 `quality = 0`이 되어야 한다.
6. Backstage Pass는 `quality = 50`이어도 콘서트 이후에는 0으로 떨어져야 한다.
7. `Aged Brie`는 만료 후에도 50을 초과하지 않는 범위에서 증가한다.
8. `Conjured`는 판매 기한 전 `quality -2`, 판매 기한 이후 `quality -4`를 적용한다.
9. `Conjured`의 감소량이 현재 `quality`보다 커도 결과는 0으로 제한한다.
10. `Conjured` 식별 정책은 이름이 `Conjured`로 시작하는 모든 아이템으로 정하고 테스트로 고정하는 것을 권장한다.
11. 특수 이름과 정확히 일치하지 않는 아이템은 Normal 규칙을 적용한다.
12. 빈 `items` 벡터에 대해 `updateQuality()` 호출 시 예외 없이 종료되어야 한다.
13. 여러 아이템이 섞여 있어도 한 아이템의 처리가 다른 아이템의 상태에 영향을 주면 안 된다.

## 7. 커버리지 목표

| 지표 | 목표 | 비고 |
|---|---:|---|
| 라인 커버리지 | 90% 이상 | `GildedRose.cpp` 기준 |
| 함수 커버리지 | 100% | 생성자 및 `updateQuality()` 포함 |
| 브랜치 커버리지 | 90% 이상 권장 | 아이템 타입별 조건 분기와 경계 조건 중심 |
| 요구사항 커버리지 | 100% | 명세의 모든 bullet을 하나 이상의 테스트와 연결 |

커버리지 제외 대상은 빌드 산출물, 외부 의존성, Google Test, ApprovalTests, CMake 생성 파일이다. 분석 대상은 원칙적으로 `cpp/src`와 `cpp/include`의 프로덕션 코드로 제한한다.

## 8. gcov/lcov 측정 전략

### 8.1 빌드 옵션

GCC 또는 MinGW GCC 환경에서는 Debug 빌드에 커버리지 플래그를 추가한다.

```bash
cmake -S cpp -B cpp/build-coverage -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="--coverage -O0 -g" \
  -DCMAKE_EXE_LINKER_FLAGS="--coverage"
cmake --build cpp/build-coverage
ctest --test-dir cpp/build-coverage --output-on-failure
```

CMake에 별도 옵션을 추가할 수 있다면 다음과 같은 방식으로 `ENABLE_COVERAGE` 옵션을 두는 것을 권장한다.

```cmake
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)

if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(gilded_rose PRIVATE --coverage -O0 -g)
    target_link_options(gilded_rose PRIVATE --coverage)
    target_compile_options(gilded_rose_test PRIVATE --coverage -O0 -g)
    target_link_options(gilded_rose_test PRIVATE --coverage)
endif()
```

### 8.2 lcov 리포트 생성

```bash
lcov --capture --directory cpp/build-coverage --output-file coverage.info
lcov --remove coverage.info "*/_deps/*" "*/test/*" "*/build*/*" --output-file coverage.filtered.info
genhtml coverage.filtered.info --output-directory coverage-report
```

생성된 HTML 리포트에서 다음 항목을 우선 확인한다.

1. `GildedRose.cpp`의 미실행 라인.
2. Normal, Aged Brie, Backstage Pass, Sulfuras, Conjured 분기별 실행 여부.
3. `quality == 0`, `quality == 50`, `sellIn == 0`, `sellIn < 0` 조건의 브랜치 실행 여부.
4. Backstage Pass의 `sellIn < 11`, `sellIn < 6`, 콘서트 이후 0 처리 분기.

## 9. 커버리지 개선 전략

1. 먼저 P0 테스트를 작성해 핵심 요구사항을 모두 통과시킨다.
2. lcov 리포트에서 미실행 분기를 확인하고 P1 경계값 테스트를 추가한다.
3. 브랜치 커버리지가 낮으면 조건문 단위로 true/false 양쪽을 검증하는 테스트를 추가한다.
4. Backstage Pass는 `sellIn = 11, 10, 6, 5, 0, -1`을 모두 포함해 임계값 분기를 닫는다.
5. 품질 상한/하한은 `quality = 0, 1, 49, 50` 조합으로 누락 없이 검증한다.
6. Conjured 구현 후에는 일반 아이템과 동일한 경계값을 재사용하되 감소량만 다르게 기대한다.
7. 커버리지 수치만 맞추기 위한 테스트보다 요구사항 bullet과 직접 연결되는 테스트를 우선한다.
8. 여러 날 시뮬레이션 테스트는 잔여 리스크 확인용으로 사용하고, 실패 원인이 모호해지지 않도록 핵심 규칙 테스트와 분리한다.

## 10. 완료 기준

테스트 계획의 완료 기준은 다음과 같다.

1. P0 테스트가 모두 작성되고 통과한다.
2. P1 경계값 테스트가 모두 작성되고 통과한다.
3. Conjured 식별 정책이 테스트명과 테스트 데이터로 명확히 고정된다.
4. `GildedRose.cpp` 기준 라인 커버리지 90% 이상을 달성한다.
5. 핵심 조건문의 브랜치 커버리지 90% 이상을 달성하거나, 미달성 분기에 대한 예외 사유를 문서화한다.
6. 테스트 실행 및 커버리지 측정 명령이 CI 또는 로컬 개발 절차에 포함된다.
