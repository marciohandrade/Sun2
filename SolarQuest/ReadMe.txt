========================================================================
    ���� ���� ���̺귯�� : SolarQuest ������Ʈ ����
========================================================================


{{���� ����}}

2006.3.27
1. QuestCondition_Concrete�� ���� : ���� ���� ��ü���� ������ �ϴ� ���� ����
2. 


{{Ŭ���̾�Ʈ�� ������ ������ ������}}
* Ŭ���̾�Ʈ
- QuestManager�� Event()�Լ��� �̺�Ʈ�� �Ͼ�� ������ ȣ������� �Ѵ�.
  ( ������insert,delete,update, hero LV, ����Ʈ �Ϸ�, �ð�üũ, ��üũ ���)
- QuestManager_ConcreteŬ������ OnEvent()�Լ��� overloading�Ͽ� Quest�� ���� ���� �̺�Ʈ�� catch�Ѵ�.
- ��������, ���������� ���� ������ QUEST_FAILED_SYN, QUEST_COMPLETE_SYN�� ������.
- FAILED_ACK�� ������ -> �������̽��� ���и� ǥ���Ѵ�.
- COMPLETE_ACK�� ������ -> �������̽��� �Ϸ� ǥ�ø� ���ش�.
- NAK�̸� ���� �޼��� ���
- �÷��̾ ����Ʈ�� �����ϸ� QUEST_ABANDON_SYN�� ������.
- �÷��̾ ����Ʈ�� ����ޱ��ϸ� QUEST_REWARD_SYN�� ������.

* ����
- �÷��̾ ������ ��� ���� ��, DB�κ��� Quest������ �ް�, QuestStreamManager�� �־��ش�.
- QuestStreamManager�� ���� �̿��� �������� ����Ʈ�� QuestManager�� �־��ش�, �׸��� Ŭ���̾�Ʈ�� Quest������ �ش�.
- Ŭ���̾�Ʈ���� QUEST_ACCEPT_SYN ��Ŷ�� ���� ���, QuestManager�� CanAccept()�� ȣ���ѿ� ACK/NAK�� ������.
- QuestManager�� Event()�Լ��� ���� ų�� ó���Ѵ�. ���� ų�� Ŭ���̾�Ʈ�� QCode�� �ش��ϴ� ���� ų���� �����ش�.(QUEST_MONSTER_KILL_CMD)
- QUEST_ABANDON_SYN������ �� QuestManager���� �������� Quest�� �����Ѵ�.
- QUEST_REWARD_SYN�� ���� �� QuestManager���� Quest�� �������ְ�, �Ϸ� ����Ʈ�� �����Ѵ�.(QuestStreamManager�� ����)
  Quest������ �о ����ũ��Ʈ���� ������ �ش�.
- 