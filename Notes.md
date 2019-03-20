# Version 0.0.1

# 참고사항
## 인터럽트 실험
### Timer 인터럽트를 이용하여 IRQ, FIQ 모두 설정했을 때 현상 관찰.
1. IRQ, FIQ 둘 다 설정되었을 때 일단 먼저 호출되는 건 FIQ handler이다. 항상 그러한지는 알 수 없으나 실험 결과는 그러하다.
2. FIQ handler에서 IRQ Basic pending 레지스터를 검사해보니 Timer의 pending bit가 1로 설정되어 있었다.
   FIQ만 설정했을 때는 IRQ Basic pending 레지스터의 pending bit가 0으로 설정된다.
3. FIQ handler에서 Timer의 IRQ 레지스터를 clear하기 때문에 IRQ handler가 호출되지 않는 것인지 알아보기 위해서 
   Timer의 IRQ 레지스터를 clear하지 않으니 계속해서 FIQ handler가 무한 호출된다. 즉, interrupt가 처리되지 않은 것으로 
   판단되어 계속해서 FIQ handler가 다시 호출되는 것이다. FIQ가 처리되지 않으니 IRQ handler는 호출되지 않았다.
4. 만약 FIQ handler만 정상 처리된 것으로 처리할 수 있고 IRQ의 pending bit가 여전히 1이라면 IRQ handler가 호출되지 않을까 싶지만
   Timer의 IRQ 레지스터를 Clear하면 FIQ, IRQ 모두 처리되어버리기 때문에 테스트해볼 수 없었다.
5. 문서에는 FIQ를 enable시키면 IRQ 설정은 disable 시키라고 되어있다. 그렇게 하지 않으면 둘 다 발생된다고 한다.
