# Gilded Rose `updateQuality()` 코드 품질 분석

## 1. 분석 대상

- 대상 파일: `cpp/include/GildedRose.h`, `cpp/src/GildedRose.cpp`
- 대상 함수: `GildedRose::updateQuality()`
- 관점: SOLID, Code Smell, C++17 리팩토링 가능성

현재 `updateQuality()`는 모든 아이템의 식별, 판매 기한 감소, 품질 증감, 품질 상한/하한 보정, 예외 아이템 처리까지 하나의 메서드 안에서 직접 수행한다. 동작은 작은 카타 예제 안에서는 이해할 수 있지만, 새 아이템 타입이 추가될수록 조건문이 더 깊어지고 기존 분기를 수정해야 하므로 변경 비용과 회귀 위험이 커진다.

## 2. 문제점 분석 표

| 문제점 | 위반 원칙/스멜 | 영향 | 개선 방향 | 우선순위 |
|---|---|---|---|---:|
| `updateQuality()`가 아이템 순회, 타입 판별, `sellIn` 변경, `quality` 계산, 경계값 보정, 예외 처리까지 모두 담당한다. | SRP 위반, Long Method | 하나의 요구사항 변경이 함수 전체에 영향을 주며, 특정 아이템 규칙만 독립적으로 테스트하거나 교체하기 어렵다. | 순회 로직은 `GildedRose`에 남기고, 아이템별 품질 갱신 규칙은 별도 함수나 정책 객체로 분리한다. | 1 |
| 아이템 타입이 문자열 비교 분기로 하드코딩되어 있다. 새 타입을 추가하려면 기존 조건문 내부를 수정해야 한다. | OCP 위반, Switch/Conditional Logic Smell | `Conjured` 같은 신규 타입 추가 시 기존 `Aged Brie`, `Backstage Pass`, `Sulfuras`, 일반 아이템 로직을 건드리게 되어 회귀 위험이 커진다. | `ItemUpdater` 전략 인터페이스, 이름 기반 팩토리, 또는 규칙 테이블을 도입해 새 타입은 새 정책 추가로 확장한다. | 1 |
| `"Aged Brie"`, `"Backstage passes to a TAFKAL80ETC concert"`, `"Sulfuras, Hand of Ragnaros"` 문자열 리터럴이 조건문에 반복된다. | Magic String, Duplicated Code | 오탈자에 취약하고, 아이템 식별 정책이 여러 위치에 흩어져 변경 지점을 찾기 어렵다. | `constexpr std::string_view` 상수로 아이템 이름을 정의하고, `isAgedBrie()`, `isBackstagePass()`, `isSulfuras()` 같은 의미 있는 판별 함수로 감싼다. | 2 |
| `0`, `50`, `11`, `6` 같은 숫자가 도메인 의미 없이 직접 사용된다. | Magic Number | `50`이 품질 상한인지, `11`과 `6`이 Backstage Pass 임계값인지 코드를 읽으며 추론해야 한다. 경계값 변경 시 누락 가능성이 있다. | `MinQuality = 0`, `MaxQuality = 50`, `BackstageFirstThreshold = 10`, `BackstageSecondThreshold = 5`처럼 규칙 언어에 맞는 상수로 치환한다. 현재 `< 11`, `< 6`은 `<= 10`, `<= 5` 의미가 드러나게 정리한다. | 2 |
| 품질 증가 전마다 `quality < 50`, 감소 전마다 `quality > 0` 검사가 반복된다. | Duplicated Code, Primitive Obsession | 상한/하한 보정 규칙이 중복되어 새 감소량 또는 증가량을 추가할 때 같은 방어 로직을 반복 작성하게 된다. | `increaseQuality(Item&, int amount)`, `decreaseQuality(Item&, int amount)`, `setQuality(Item&, int value)` 같은 헬퍼로 품질 불변식을 한곳에 모은다. | 2 |
| `if`가 중첩되어 일반 아이템, 증가형 아이템, 예외 아이템, 만료 후 로직이 한 흐름에 섞여 있다. | 조건문 복잡도, Arrow Code | 현재는 56줄 수준이지만 의미 단위가 뒤섞여 인지 복잡도가 높다. 조건 하나를 바꾸면 반대 분기 동작까지 함께 확인해야 한다. | 조기 반환이 가능한 `Sulfuras` 처리, 아이템별 작은 갱신 함수, 또는 전략 디스패치로 중첩 깊이를 낮춘다. | 3 |
| `items[i]` 접근이 반복되고 인덱스 기반 루프를 사용한다. | Readability Smell | 같은 요소 접근이 과도하게 반복되어 핵심 규칙보다 컨테이너 접근 코드가 더 눈에 띈다. | C++17 범위 기반 `for (auto& item : items)`를 사용하고, `item` 변수명으로 규칙 중심의 코드를 만든다. | 3 |
| `Backstage Pass`의 증가 규칙이 단계별 조건으로 직접 박혀 있다. | Feature Envy, Rule Scattering | 임계값이나 증가량이 늘어나면 조건문이 계속 추가된다. 같은 `quality < 50` 검사도 단계마다 반복된다. | `std::array` 기반 규칙 테이블로 `{threshold, increment}`를 표현하거나, `daysRemaining`에 따른 증가량 계산 함수를 분리한다. | 3 |
| `items`가 `GildedRose`의 public reference 멤버로 노출되어 있다. | Encapsulation 약화 | 외부에서 아이템 목록을 직접 변경할 수 있어 `GildedRose`가 갱신 전후 상태를 통제하기 어렵다. 단, 카타의 기존 공개 인터페이스라 변경은 신중해야 한다. | 현재 과제에서는 `Item`과 공개 계약을 유지하고, 리팩토링은 `updateQuality()` 내부 구조 개선에 집중한다. 장기적으로는 캡슐화 개선을 별도 과제로 분리한다. | 5 |

## 3. C++17 스타일 개선 방향 요약

1. 가장 먼저 품질 경계값과 아이템 이름을 상수화한다.
   - 예: `constexpr int MinQuality = 0;`, `constexpr int MaxQuality = 50;`
   - 예: `constexpr std::string_view AgedBrieName = "Aged Brie";`
   - 이 단계는 동작을 바꾸지 않으면서 가독성과 변경 안정성을 즉시 높인다.

2. `quality` 변경 헬퍼를 도입한다.
   - `increaseQuality(item, amount)`와 `decreaseQuality(item, amount)`가 `0..50` 범위를 보장하게 한다.
   - `Sulfuras`는 일반 상한 50의 예외이므로 헬퍼 적용 대상에서 제외하거나 별도 정책에서 조기 반환한다.

3. 아이템별 규칙 함수를 분리한다.
   - 예: `updateNormal(item)`, `updateAgedBrie(item)`, `updateBackstagePass(item)`, `updateSulfuras(item)`
   - `GildedRose::updateQuality()`는 `for (auto& item : items)`와 디스패치만 담당하게 만들어 SRP 위반을 줄인다.

4. 전략 패턴을 적용할 수 있다.
   - `class ItemUpdater { public: virtual void update(Item&) const = 0; };`
   - 이름 기반 팩토리에서 `NormalUpdater`, `AgedBrieUpdater`, `BackstagePassUpdater`, `SulfurasUpdater`, `ConjuredUpdater`를 선택한다.
   - 신규 아이템 추가 시 기존 정책을 수정하지 않고 새 전략을 추가할 수 있어 OCP에 가장 잘 맞는다.
   - 단, 이 카타 규모에서는 동적 할당과 가상 호출이 과할 수 있으므로 테스트가 충분해진 뒤 적용하는 편이 안전하다.

5. 테이블 기반 접근도 가능하다.
   - `Backstage Pass`처럼 임계값과 증가량이 명확한 규칙은 `std::array`로 표현할 수 있다.
   - 예: `sellIn <= 5`이면 `+3`, `sellIn <= 10`이면 `+2`, 그 외 `+1`처럼 규칙을 데이터로 분리한다.
   - 단순한 규칙에는 전략 클래스보다 가볍고 C++17 코드에서도 읽기 쉽다.

6. `std::variant` 기반 모델은 장기 개선안으로 검토할 수 있다.
   - `NormalItem`, `AgedBrie`, `BackstagePass`, `Sulfuras` 같은 타입을 `std::variant`로 표현하면 컴파일 타임 디스패치가 가능하다.
   - 그러나 현재 `Item` 구조체와 public 필드는 외부 계약이므로 즉시 적용하면 변경 범위가 커진다.
   - 따라서 현 단계에서는 `Item`은 유지하고, 내부 정책 계층에서만 타입 분류를 캡슐화하는 편이 적절하다.

## 4. 리팩토링 우선순위

1. **특성화 테스트 보강**
   - 구조 변경 전 현재 동작을 고정해야 한다.
   - 특히 `quality = 0`, `quality = 50`, `sellIn = 0`, `sellIn = -1`, `Sulfuras` 불변 동작, `Backstage Pass` 경계값을 우선 검증한다.

2. **상수화와 판별 함수 추출**
   - 동작 변경 없이 리스크가 낮고, 이후 리팩토링의 의도를 명확하게 만든다.
   - Magic Number와 Magic String을 먼저 제거하면 다음 단계의 분리 작업이 쉬워진다.

3. **품질 변경 헬퍼 도입**
   - 중복된 상한/하한 검사 제거 효과가 크다.
   - `quality` 불변식을 한곳에 모아 `Conjured`처럼 감소량이 큰 규칙도 안전하게 추가할 수 있다.

4. **아이템별 업데이트 함수 분리**
   - Long Method와 중첩 조건문 문제를 실질적으로 줄인다.
   - 이 단계까지는 전략 패턴 없이도 충분히 가독성과 테스트성이 좋아진다.

5. **전략 패턴 또는 테이블 기반 디스패치 도입**
   - 아이템 타입이 계속 늘어나는 요구사항이 확실할 때 적용한다.
   - `Conjured` 추가가 예정되어 있다면 전략 패턴이나 이름 기반 정책 테이블이 OCP 관점에서 유리하다.
   - 반대로 현재 타입 수가 고정이라면 작은 함수 분리 수준에서 멈추는 것이 과설계를 피하는 선택이다.

## 5. 결론

`updateQuality()`의 가장 큰 문제는 개별 코드 조각의 복잡함보다, 모든 비즈니스 규칙이 하나의 조건문 덩어리에 결합되어 있다는 점이다. 따라서 리팩토링은 대규모 재작성보다 테스트 고정, 상수화, 품질 변경 헬퍼, 아이템별 함수 분리 순서로 진행하는 것이 안전하다.

장기적으로 아이템 종류가 늘어날 가능성이 높다면 전략 패턴이 가장 명확한 OCP 해법이다. 다만 현재 C++17 카타의 크기와 `Item` 공개 계약을 고려하면, 먼저 작은 함수와 규칙 테이블로 구조를 정리한 뒤 필요할 때 전략 객체로 확장하는 접근이 적절하다.
