================================================================================
                   ������ WASEDA �~ GREE Samurai coding ������
================================================================================

                         WASEDA �~ GREE Samurai coding ��
         �O���[������Зl�̋��^�ŊJ�Â����v���O���~���O�R���e�X�g�ł��D
                     �ŋ���AI���쐬���ėD����ڎw���܂��傤�I

================================================================================

��README
���̃t�@�C���ł́C��ɃQ�[���̋N�����@�ɂ��Đ������܂��D
�R���e�X�g�C���̏ڍׂɂ��ẮC�K��web�̃h�L�������g���Q�Ƃ��������D


���Q�[���{�̂̋N�����@
���̃t�@�C���̂���t�H���_���J�����g�f�B���N�g���Ƃ��āC
�ȉ��̃R�}���h�����s���邱�ƂŃQ�[�����N�����܂��D

> java -jar SamuraiCoding2011-2.0.4.jar [options] command

option �Ƃ��Ĉȉ��̈������w��ł��܂��D
 -a <arg> �X�R�A���M�p�̃A�N�Z�X�g�[�N�����w�肵�܂�.
 -c <arg> �e�L�����N�^�[�̏������Ԃ��w�肵�܂��D�f�t�H���g�l�� 1000[ms] �ł��D
 -d       �f�o�b�O���[�h�ŋN�����܂��D
 -f <arg> �Q�[���X�s�[�h�𒲐����� FPS ���w�肵�܂��D�f�t�H���g�l�� 30[fps]�ł��D
 -h       �w���v��\�����܂��D
 -n       CUI���[�h�ŋN�����܂�.
 -t <arg> �Q�[���̃^�[�������w�肵�܂��D�f�t�H���g�l�� 200[turns] �ł��D
 -u       ���[�U�[���[�h�ŋN�����܂��D
          (AI���g�p�����C�L�[�{�[�h����ŃQ�[�����i�s���܂�)
		Player 0 Samurai ��,��,��,��
		Player 0 Dog      C, V, Z, X
		Player 1 Samurai  3, 4, 1, 2
		Player 1 Dog      7, 8, 5, 6
		Player 2 Samurai  E, R, Q, W
		Player 2 Dog      U, I, T, Y
		Player 3 Samurai  D, F, A, S
		Player 3 Dog      J, K, G, H

command �ɋN������AI�̎��s�R�}���h���w�肵�܂��D�R�}���h�� "" �Ŋ����Ďw�肵�܂��D
(�R�}���h��j
Java�F
java -jar SamuraiCoding2011-2.0.4.jar "java -cp SampleAI_Java_Japanese/bin jp.ac.waseda.cs.washi.samurai.main.Main"

C�F
java -jar SamuraiCoding2011-2.0.4.jar "ai.exe"

Ruby�F
java -jar SamuraiCoding2011-2.0.4.jar "ruby ai.rb"


���T���v��AI(Java)
�T���v���Ƃ���Java�ŋL�q����AI�𓯍����Ă��܂��D
�t���̃o�b�`�t�@�C���Ŏ��s���Ă��������D

SampleAI_Java_Japanese/compile.bat(.sh):
�@�T���v��AI���N���[���C�R���p�C�����܂��D
SampleAI_Java_Japanese/run.bat(.sh):
�@�T���v��AI���w�肵�ăQ�[�����N�����܂��D

���X�V����
2.0.4   SHOGUN���[�h��Ԃ̎��ԏ����C��, �o�O�C��, -n�I�v�V�����̒ǉ�(2012.1.1)
2.0.3   �ŐV�o�[�W�����̖{�̂���̂݃X�R�A���M���󂯕t����悤�C��(2011.12.26)
2.0.1   �X�R�A���M�@�\�̒ǉ�(2011.12.24)
2.0.0   �Q�[���{�̂�\�I�p�ɃA�b�v�f�[�g(2011.12.23)
1.0.0   ���񃊃��[�X(2011.12.16)

����m�F�FJRE6(windowsXP, windows7, MacOSX10.6, MacOSX10.7, debian squeeze)
--------------------------------------------------------------------------------
WASEDA �~ GREE Samurai coding(http://www.washi.cs.waseda.ac.jp/samurai_coding/)
