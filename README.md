# Gilded Rose

Gilded Rose 는 게임 "World of Warcraft"에 나오는 여관 이름입니다.  
아이템은 퀄리티 속성을 가지고 있으며, 예외 사항이 있는 아이템들이 있습니다.  
- 아이템은 [아이템 이름, 퀄리티, 유통기한]의 속성을 가집니다.  
- 아이템의 퀄리티는 0 이상이고 아이템의 퀄리티는 하루가 지날 때마다 1씩 줄어듭니다.  
- 유통 기한이 지난 아이템의 퀄리티는 2배의 속도로 떨어집니다.  
- 퀄리티는 최대값이 50입니다.  

### Aged Brie, Backstage Pass, Sulfuras 의 예외적 규칙  

- Aged Brie는 하루가 지날 때마다 퀄리티가 1씩 증가합니다.  
- 유통기한이 지나면 퀄리티가 2씩 증가합니다.  
- Backstage Pass 는 유통기한(콘서트일)이 다가올수록 퀄리티가 증가합니다.  
  유통기한이 11일 이상일 때는 1, 10일 이하일 때는 2, 5일 이하일 때는 3씩 증가하지만  
  콘서트 날이 지나면 퀄리티는 0이 됩니다.  
- Sulfuras의 퀄리티는 변화가 없습니다.  
  
---------------------------------------------------------  
## 실습 목적 :  테스트 코드 작성을 통한 legacy code 이해

### 프로젝트 구조 (C++)

```
cpp/
├── CMakeLists.txt          # CMake 빌드 설정 (Google Test 자동 다운로드)
├── include/
│   ├── Item.h              # Item 클래스 (수정 불가)
│   └── GildedRose.h        # GildedRose 클래스 헤더
├── src/
│   └── GildedRose.cpp      # updateQuality() 구현
└── test/
    └── GildedRoseTest.cpp  # Google Test 기반 테스트
```

### 빌드 및 테스트 실행

```bash
cd cpp
cmake -B build
cmake --build build
ctest --test-dir build
```

### GildedRoseTest.cpp : failed test 수정
- updateQuality() 의 test case를 추가 작성해 보세요.
- unit test 내용 : 문서상에 나타난 동작들 확인

### unit test VS golden-master test 비교

### To-Do list
- 1~10 Test Code 작성완료 / 2026-05-13 17:31

- GildedRose.cpp Refectoring 6단계 진행 완료
 1. Sulfuras Loop 탈출코드 추가
 2. 퀄리티 상승, 하락 함수분리 및 부정문 사용으로 인한 다중 if문 개선
 3. Items[i] 보다는 참조문 사용하여 한눈에 들어오게 개선
 4~5. 유통기한 감소 위치 변경 및 아이템별 로직 완전분리
 6. 반복문 내 조건 단순화 및 특수 아이템 관련 동작 함수분리

- Approval (Golden master) Test 완료
 1. cmake 텍스트 파일 수정
 2. approval Test 신규생성 (테스트 조건 임의 입력)
 3. Approval Test 진행
 4. Golden master 파일 생성 후 Received 파일 approved 파일로 내용 옮겨적은 후 다시 테스트
 5. Received 파일 삭제 확인
 6. PDF 참고하여 빠진 부분이나 추가 수정할 부분 리펙토링 진행
 7. F&B 아이템 추가
 8. TestCode 추가
 9. 단위테스트완료
 10. Approval Test 완료 - Golden Master File Upload