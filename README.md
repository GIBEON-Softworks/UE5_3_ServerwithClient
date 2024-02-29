# UE5_3_ServerwithClient
언리얼 서버와 클라이언트 통신 방식에 대한 메인 작업

| 작업일 | 작업 사항 | 내용 |
|---|---|---|
|24.02.07| Unreal Client request Server[Cpp] | 언리얼 클라이언트 서버 로비 및 로그인 IP등 기본 포트 소켓통신|
|24.02.14 - 24.02.15| Unreal Client UI | 기존 프로젝트 병합에 의한 bug fix 및 UI 결합 및 수정 | 
|24.02.21 - 24.02.28| Unreal <-> Server Google Login with Basic Login & MakeAccount | 회원가입, 일반 로그인, 클릭 스코어, 랭킹 시스템, 구글 로그인 update |


# UE5 사용시 주의 사항
### Debuging
- 240214 - 
1. Blueprint  C++로 만들어준 자식 블루프린트의 경우 명시적으로 꼭 ' _C' 주소를 넣어주고 뒤에 붙여주도록 하자. 
  ```cpp
  static ConstructorHelpers::FClassFinder<APawn> NetBp(TEXT("'Blueprint'/Game/HitUP/Widgets/Actors/BPC_NetActor.BPC_NetActor_C'"));
  ```

### Tasks
- 240216 -
1. UI 병합 및 레벨 연동
2. 구글 로그인을 위한 작업 예정 ( 보내야 할 정보 및 서버와의 공유 예정 )
3. 기본 회원가입 및 소셜 로그인(구글)
4. 소셜에 해당 아이디를 인증하고 나면 클라에서 동기화 루프 해당 아이디가 정확하게 받았다면 허락 해줘서 들어오게 한다?
5. 로그인 후 해당 IP? 또는 토큰? 등을 파악해서 닉네임이 없다면 만들게 한다. 
