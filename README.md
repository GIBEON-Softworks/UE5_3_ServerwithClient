# UE5_3_ServerwithClient
언리얼 서버와 클라이언트 통신 방식에 대한 메인 작업

| 작업일 | 작업 사항 | 내용 |
|---|---|---|
|24.02.07| Unreal Client request Server[Cpp] | 언리얼 클라이언트 서버 로비 및 로그인 IP등 기본 포트 소켓통신|



# UE5 사용시 주의 사항
### Debuging
- 240214 - 
1. Blueprint  C++로 만들어준 자식 블루프린트의 경우 명시적으로 꼭 ' _C' 주소를 넣어주고 뒤에 붙여주도록 하자. 
  ```cpp
  static ConstructorHelpers::FClassFinder<APawn> NetBp(TEXT("'Blueprint'/Game/HitUP/Widgets/Actors/BPC_NetActor.BPC_NetActor_C'"));
  ```
2. 
