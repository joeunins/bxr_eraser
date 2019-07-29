BXR_ERASER (data wiping solution)
=================================
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/eda5325fb8144c9bba8e48eaa852d300)](https://app.codacy.com/app/joeunins/bxr_eraser?utm_source=github.com&utm_medium=referral&utm_content=joeunins/bxr_eraser&utm_campaign=Badge_Grade_Dashboard)
[![Build Status](https://travis-ci.org/joeunins/bxr_eraser.svg?branch=master)](https://travis-ci.org/joeunins/bxr_eraser)

![main_red](https://user-images.githubusercontent.com/40619246/61026393-246a4900-a3ee-11e9-8ca5-812c288f83da.png)
Blue X-ray Eraser v1.0
----------------------

Table of content
----------------

* What is BXR_ERASER?
  * Purpose of project
  * program description
* installation
* License
* Link

기술개발 목적
-------------
최근 정부는 국가적으로 개방형 OS를 적용하기 위해  ‘15년 과지정통부의 개방형 OS 시범사업(7개기관)을 통해 사업을 추진하고 있으며,  국방부는 2017년 ‘개방형 OS의 군 도입방향과 과제’라는 방향으로 포럼에서 제시하였으며, 관련 사업을 2개 추진하고 있어, 개방형 OS로의 전환이 가속화 되고 있는 추세이다. 이에 개방형 S/W  생태계 조성을 위한 국가 시책에 따라 다양한 공개 SW의 개발이 필요로 한 상황이다. 본 기술의 목적은 이러한 국가 정책 방향에 따라, 본 제안사의 전문분야인 개인정보 검출 및 파일 완전 삭제를 통해 개방형 S/W의 생태계 조성을 위한 것이 기술 개발의 목적이다.

기술 설명
--------
BXR_ERASER는 개방형 OS환경에서의 HDD 완전삭제를 통한 정보 유출 방지 솔루션입니다. 기존 파일의 Data영역을 초기화 한 후 삭제하는 간편 삭제, 미국 국방부(DoD)에서 지정한 3번 이상 덧씌우는 방식의 삭제 등 데이터 재기록 작업을 통해 흔적을 통한 복구를 차단하는 삭제 프로그램입니다.

설치 방법
---------
git clone URL
#### compilation
make
#### execution
./bxre

Ubuntu 18.04.1 버전에서 개발 및 테스트가 완료된 상태입니다.

라이센스
--------
BXR_ERASER는 GPL 오픈 소스 라이센스의 조건에 따라 사용이 허가되며 무료로 사용할 수 있습니다.

링크
----
[조은아이앤에스](http://www.joeunins.com/src/home.html)


