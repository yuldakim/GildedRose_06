# Gilded Rose 결함 관리 보고서

## 1. 작성 목적

이 문서는 Gilded Rose 프로젝트의 결함을 일관되게 분류, 보고, 추적, 검증하기 위한 QA 결함 관리 기준을 정의한다.

기준 문서는 `docs/defect_list.md`, `docs/requirements_analysis.md`, `docs/test_plan.md`이며, 대상 아이템 타입은 Normal, Aged Brie, Backstage Pass, Sulfuras, Conjured 5종이다.

## 2. 결함 분류 체계

### 2.1 Severity 정의

| Severity | 정의 | 처리 기준 |
|---|---|---|
| Critical | 핵심 재고 가치 계산이 심각하게 깨져 배포 또는 릴리스를 중단해야 하는 결함 | 즉시 수정, 회귀 테스트 필수, 릴리스 차단 |
| Major | 특정 아이템 타입 또는 핵심 경계값에서 요구사항과 다른 결과가 발생하는 결함 | 우선 수정, 관련 테스트 추가 또는 보강 |
| Minor | 기능 결과의 직접 오류는 아니지만 테스트, 빌드, 운영 절차에 영향을 주는 결함 | 계획된 수정 또는 문서화 후 추적 |
| Info | 결함은 아니지만 개선, 관찰, 리스크 기록이 필요한 항목 | 백로그 관리 또는 참고 기록 |

### 2.2 ItemType 정의

| ItemType | 식별 기준 | 주요 규칙 |
|---|---|---|
| Normal | 특수 아이템이 아닌 일반 아이템 | 매일 `sellIn -1`, `quality -1`, 판매 기한 이후 `quality -2` |
| Aged Brie | `Aged Brie` | 시간이 지날수록 `quality` 증가, 판매 기한 이후 증가량 2배 |
| Backstage Pass | `Backstage passes to a TAFKAL80ETC concert` | 콘서트 임박 구간별 증가, 콘서트 이후 `quality = 0` |
| Sulfuras | `Sulfuras, Hand of Ragnaros` | `sellIn`, `quality` 모두 불변, `quality = 80` 예외 허용 |
| Conjured | `Conjured` 접두어 아이템 | Normal보다 2배 빠르게 감소, 판매 기한 이후 `quality -4` |

### 2.3 Severity x ItemType 매트릭스

| Severity \ ItemType | Normal | Aged Brie | Backstage Pass | Sulfuras | Conjured |
|---|---|---|---|---|---|
| Critical | 모든 일반 아이템의 `quality` 하한이 깨져 음수가 발생함 | `quality` 상한 50이 전역적으로 깨져 데이터 오염 발생 | 콘서트 이후에도 `quality`가 0이 되지 않아 대량 오산 발생 | 전설 아이템 값이 매일 변경되어 불변 요구사항이 깨짐 | 모든 Conjured 아이템이 잘못된 타입으로 처리되어 핵심 신규 요구사항 전체 실패 |
| Major | 판매 기한 이후 감소량이 2가 아닌 값으로 계산됨 | 만료 후 증가량 또는 상한 처리 오류 | 10일/5일 경계값 증가량 오류 | `sellIn` 또는 `quality` 중 하나가 변경됨 | 판매 기한 전 `-2`, 만료 후 `-4` 규칙 누락 |
| Minor | 특정 경계값 테스트 누락 또는 테스트명/데이터 불명확 | 상한 경계 테스트 누락 | 임계값 일부 조합 테스트 누락 | `quality = 80` 예외 문서화 부족 | 접두어 식별 정책 문서화 또는 테스트 누락 |
| Info | 리팩토링 후보, 중복 조건, 매직 넘버 | 테스트 가독성 개선 | 규칙 테이블화 후보 | 특수 예외 주석 보강 | 향후 신규 Conjured 이름 추가 리스크 |

## 3. 현재 결함 현황

`docs/defect_list.md` 기준 현재 기능 결함은 해결 완료 상태이며, 빌드/테스트 실행 경로 이슈는 문서화되어 있다.

| ID | Severity | ItemType | 상태 | 요약 |
|---|---|---|---|---|
| DEF-001 | Major | Conjured | Resolved | 판매 기한 전 Conjured 아이템이 Normal처럼 `quality -1`로 처리되던 결함 |
| DEF-002 | Major | Conjured | Resolved | 판매 기한 당일/이후 Conjured 아이템이 `quality -4`가 아닌 `-2`로 처리되던 결함 |
| DEF-003 | Minor | Build/Test Infrastructure | Open/Documented | 루트 `build` 경로가 유효한 CMake 빌드 디렉터리가 아닌 실행 절차 이슈 |

## 4. 결함 보고서 템플릿

아래 템플릿은 결함 등록 시 GitHub Issue 또는 Markdown 문서에 동일하게 사용한다.

```markdown
## Defect ID

- ID:
- Title:
- Severity: Critical | Major | Minor | Info
- ItemType: Normal | Aged Brie | Backstage Pass | Sulfuras | Conjured | Build/Test Infrastructure
- Status: Open | In Progress | Resolved | Verified | Closed | Won't Fix
- Reporter:
- Assignee:
- Found In:
- Fixed In:

## Reproduction

1.
2.
3.

## Expected

- 기대 결과:
- 관련 요구사항:

## Actual

- 실제 결과:
- 실패 로그 또는 관찰 결과:

## Root Cause

- 원인 위치:
- 원인 설명:

## Fix

- 수정 요약:
- 변경 파일:
- 영향 범위:

## Verification

- 검증 테스트:
- 빌드 명령:
- 테스트 명령:
- 커버리지 확인:
- 회귀 리스크:
```

### 4.1 필수 작성 기준

1. `Reproduction`에는 입력 아이템 이름, `sellIn`, `quality`, 호출 횟수를 반드시 포함한다.
2. `Expected`는 `docs/requirements_analysis.md`의 규칙 또는 `docs/test_plan.md`의 테스트 케이스와 연결한다.
3. `Actual`은 실패한 `EXPECT_EQ` 값, CTest 로그, 승인 테스트 diff 중 하나 이상을 포함한다.
4. `Root Cause`는 가능한 경우 파일명과 함수명을 명시한다.
5. `Fix`는 제품 코드 수정과 테스트 보강을 구분해 기록한다.
6. `Verification`은 빌드, 단위 테스트, 커버리지, 회귀 테스트 결과를 포함한다.

## 5. 품질 메트릭 수집 계획

### 5.1 테스트 통과율

| 지표 | 산식 | 수집 방법 | 목표 |
|---|---|---|---:|
| Test Pass Rate | `Passed Tests / Total Tests * 100` | CTest, Google Test, CI 결과 | 100% |
| Failed Test Count | 실패 테스트 수 | `ctest --output-on-failure` 로그 | 0 |
| Regression Failure Count | 과거 Green 테스트의 재실패 수 | CI 히스토리, GitHub Checks | 0 |

권장 C++ 수집 명령:

```bash
cmake --build cpp/build --config Debug
ctest --test-dir cpp/build --build-config Debug --output-on-failure
```

### 5.2 커버리지

| 언어 | 도구 | 수집 대상 | 목표 |
|---|---|---|---:|
| C++ | gcov/lcov, genhtml | `cpp/src`, `cpp/include` | 라인 90% 이상, 브랜치 90% 이상 권장 |
| Java | JaCoCo | `src/main/java` | 라인 90% 이상, 브랜치 85% 이상 권장 |
| Python | pytest-cov | 패키지 소스 디렉터리 | 라인 90% 이상, 브랜치 85% 이상 권장 |

C++ 커버리지 예시:

```bash
cmake -S cpp -B cpp/build-coverage -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="--coverage -O0 -g" \
  -DCMAKE_EXE_LINKER_FLAGS="--coverage"
cmake --build cpp/build-coverage
ctest --test-dir cpp/build-coverage --output-on-failure
lcov --capture --directory cpp/build-coverage --output-file coverage.info
lcov --remove coverage.info "*/_deps/*" "*/test/*" "*/build*/*" --output-file coverage.filtered.info
genhtml coverage.filtered.info --output-directory coverage-report
```

Java 커버리지 예시:

```bash
./gradlew test jacocoTestReport
```

Python 커버리지 예시:

```bash
pytest --cov=src --cov-branch --cov-report=term-missing --cov-report=html
```

### 5.3 단계별 결함 발견율

| 단계 | 수집 항목 | 산식 | 활용 |
|---|---|---|---|
| 요구사항 분석 | 요구사항 모호성, 누락 규칙 | 단계 결함 수 / 전체 결함 수 | 명세 품질 개선 |
| 테스트 설계 | 경계값 누락, 테스트 데이터 오류 | 단계 결함 수 / 전체 결함 수 | 테스트 계획 보강 |
| 구현/리팩토링 | 기능 오류, 분기 오류, 불변식 위반 | 단계 결함 수 / 전체 결함 수 | 코드 리뷰 및 단위 테스트 강화 |
| 빌드/CI | 빌드 경로, 의존성, 실행 환경 오류 | 단계 결함 수 / 전체 결함 수 | 파이프라인 안정화 |
| 회귀 검증 | 재발 결함, Golden Master diff | 단계 결함 수 / 전체 결함 수 | 릴리스 차단 기준 설정 |

월간 또는 릴리스 단위로 다음 지표를 집계한다.

1. Severity별 결함 수와 비율.
2. ItemType별 결함 수와 비율.
3. 단계별 결함 발견율.
4. 결함 재오픈율.
5. 평균 수정 시간.
6. 테스트 통과율과 커버리지 추이.

## 6. GitHub Issues 연동 워크플로우

### 6.1 Issue 생성 기준

결함이 발견되면 아래 라벨을 함께 부여한다.

| 라벨 | 예시 |
|---|---|
| `severity:critical`, `severity:major`, `severity:minor`, `severity:info` | 심각도 |
| `item:normal`, `item:aged-brie`, `item:backstage-pass`, `item:sulfuras`, `item:conjured` | 아이템 타입 |
| `type:defect`, `type:test`, `type:infra` | 결함 성격 |
| `status:triage`, `status:in-progress`, `status:verified` | 처리 상태 |

### 6.2 권장 상태 흐름

```text
Open -> Triaged -> In Progress -> Resolved -> Verified -> Closed
```

상태별 완료 조건은 다음과 같다.

1. `Open`: 재현 정보가 등록되어 있다.
2. `Triaged`: Severity, ItemType, 영향 범위가 확정되어 있다.
3. `In Progress`: 담당자와 수정 방향이 정해져 있다.
4. `Resolved`: 수정 PR이 머지되었거나 수정 커밋이 포함되어 있다.
5. `Verified`: 재현 테스트, 회귀 테스트, 커버리지 확인이 완료되어 있다.
6. `Closed`: 검증 결과와 릴리스 포함 여부가 기록되어 있다.

### 6.3 PR 연동 규칙

1. PR 본문에 `Fixes #<issue-number>` 또는 `Closes #<issue-number>`를 포함한다.
2. PR에는 재현 테스트 또는 회귀 테스트를 포함한다.
3. GitHub Checks에서 빌드, 테스트, 커버리지 수집이 통과해야 한다.
4. `Critical`과 `Major` 결함은 QA 검증 코멘트 없이 닫지 않는다.

## 7. 완료 기준

결함 관리 체계의 완료 기준은 다음과 같다.

1. 모든 결함은 Severity와 ItemType으로 분류된다.
2. 모든 기능 결함은 재현, 기대, 실제, 원인, 수정, 검증 항목을 가진다.
3. 테스트 통과율과 커버리지는 CI 또는 로컬 검증 절차에서 수집 가능하다.
4. 단계별 결함 발견율을 릴리스 단위로 집계할 수 있다.
5. GitHub Issues를 사용할 경우 Issue, PR, 테스트 결과가 추적 가능하게 연결된다.
