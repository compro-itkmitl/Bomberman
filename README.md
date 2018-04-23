# Introduction
สวัสดีครับ วันนี้กลุ่ม Bomberman ของพวกเราได้ทำการสร้าง และพัฒนาโปรแกรมที่ชื่อว่า "Bomberman" ขึ้นมาโดยใช้ภาษา C++ 100% (เนื่องจาก Library SFML นั้นรองรับเฉพาะภาษา  C++) โดยตัวโปรแกรมนั้นเป็นเกมแนววางแผน ที่ต้องใช้มันสมองสามารถเล่นได้จริง และรองรับผู้เล่นได้สูงสุด 2 ผู้เล่น สามารถนำไปใช้เล่นคนเดียวหรือเล่นกับเพื่อนเวลาว่างๆเพื่อความสนุกสนานบันเทิง เพื่อฝึกทักษะด้านความคิด เพลิดเพลินได้ครับ
## How to install
สามารถดาวน์โหลดไฟล์เกมได้จาก [ที่นี่](https://github.com/compro-itkmitl/Bomberman) เมื่อดาวน์โหลดเสร็จแล้วก็ให้แตกไฟล์ตามปกติ
หลังจากแตกไฟล์แล้ว สามารถเข้าเกมได้ที่ Bomberman.exe ครับ  
![](https://github.com/compro-itkmitl/Bomberman/blob/master/picreadme/App.png)

## Main Menu
เมื่อผู้เล่นเข้าเกมมาแล้ว จะได้พบเจอกับหน้าเมนูหลักของเกม Bomberman
![](https://github.com/compro-itkmitl/Bomberman/blob/master/picreadme/Menu.png)  
หากผู้เล่นต้องการจะตั้งค่า เช่น ปิด/เปิดเสียงเพลง หรือเสียงเอฟเฟกต์ ก็สามารถเข้าไปตั้งค่าได้ที่เมนู Setting ครับ  
![](https://github.com/compro-itkmitl/Bomberman/blob/master/picreadme/Setting.png)  
สามารถกดคลิกที่รูปสี่เหลี่ยม เพื่อติ๊กเครื่องหมายถูก หรือ นำเครื่องหมายออกเพื่อปิดได้  
  
หรือถ้าอยากออกจากเกมให้กดเลือกที่ปุ่ม Quit เมื่อกดที่ปุ่ม Quit ผู้เล่นจะได้พบกับ Bomberman ที่จะถามผู้เล่นว่าต้องการออกเกมหรือไม่? หากต้องการก็กด Yes หลังจากนั้นก็จะเป็นการปิดโปรแกรมครับ  
![](https://github.com/compro-itkmitl/Bomberman/blob/master/picreadme/Quit.png)  

แต่ถ้าหากต้องการจะเล่นทันทีให้คลิกที่ Battle Mode !!  
![](https://github.com/compro-itkmitl/Bomberman/blob/master/picreadme/BattleMode.png)  



## Battle Mode
หากผู้เล่นกดเข้ามาใน Battle Mode แล้ว เกมจะให้ผู้เล่นทำการเลือกระหว่าง Singleplayer หรือ Multiplayer และเลือกแผนที่ครับ หลังจากเลือกโหมดเกมและแผนที่เรียบร้อยแล้วให้คลิกที่ปุ่ม Start ได้เลย ♥（ﾉ´∀`）  
  
![ลุยๆๆๆ](https://github.com/compro-itkmitl/Bomberman/blob/master/picreadme/Start.png)  

## How To Play

เมื่อกด Start เรียบร้อยแล้ว ผู้เล่นจะถูกส่งเข้ามาในแผนที่ โดยในแผนที่จะมีตัวละครทั้งหมด 4 ตัว นอกเหนือจากผู้เล่น ที่เหลือจะเป็นบอทครับ  
![](https://github.com/compro-itkmitl/Bomberman/blob/master/picreadme/Ingame.png)
และวัตถุต่างๆในแผนที่ทั้งหมดจะประกอบไปด้วย  
- ![](https://github.com/compro-itkmitl/Bomberman/blob/master/picreadme/Object.png)สิ่งกีดขวางทั่วไป ตั้งอยู่ทั่วไปในแผนที่ เป็นวัตถุที่สามารถทำลายได้โดยใช้ระเบิด หากโชคดี ผู้เล่นจะได้รับไอเทมอีกด้วย !  
 - ![](https://github.com/compro-itkmitl/Bomberman/blob/master/picreadme/Immortal.png)สิ่งกีดขวางที่เป็นอมตะ โดยจะตั้งปะปนอยู่กับสิ่งกีดขวางทั่วไป ไม่สามารถทำลายได้  
 - ไอเทม มีโอกาสพบได้จากการทำลายสิ่งกีดขวาง สามารถถูกทำลายได้ โดยจะแบ่งเป็น  
       *- ![](https://github.com/compro-itkmitl/Bomberman/blob/master/picreadme/bombup.png) เพิ่มจำนวนระเบิดที่สามารถวางได้พร้อมๆกัน  
       - ![](https://github.com/compro-itkmitl/Bomberman/blob/master/picreadme/bombdown.png) ลดจำนวนระเบิดที่สามารถวางได้พร้อมๆกัน  
       - ![](https://github.com/compro-itkmitl/Bomberman/blob/master/picreadme/fireup.png) เพิ่มระยะการระเบิดของผู้เล่นให้ไกลขึ้น  
       - ![](https://github.com/compro-itkmitl/Bomberman/blob/master/picreadme/firedown.png) ลดระยะการระเบิดของผู้เล่นให้สั้นลง  
       - ![](https://github.com/compro-itkmitl/Bomberman/blob/master/picreadme/speedup.png) เพิ่มความเร็วในการเคลื่อนที่ของผู้เล่น  
       - ![](https://github.com/compro-itkmitl/Bomberman/blob/master/picreadme/speeddown.png) ลดความเร็วในการเคลื่อนที่ของผู้เล่น  
       - ![](https://github.com/compro-itkmitl/Bomberman/blob/master/picreadme/piercebomb.png) ทำให้ระเบิดของผู้เล่นสามารถทำลายสิ่งกีดขวางได้ต่อเนื่อง (ระเบิดทะลุ)*  

ในการบังคับตัวละครนั้นผู้เล่นสามารถบังคับได้โดยใช้ปุ่มลูกศรบนคีย์บอร์ด เพื่อเคลื่อนที่ไปในทิศทางต่างๆ และ ปุ่ม Spacebar สำหรับการวางระเบิด  
สำหรับผู้เล่นคนที่ 2 จะสามารถบังคับได้โดยใช้ปุ่ม W A S D เพื่อเคลื่อนที่ และปุ่ม Ctrl ทางด้านซ้ายมือของคีย์บอร์ดสำหรับการวางระเบิดครับ  
โดยการวางระเบิดนั้น จะต้องรอให้ครบ 4 วินาที ระเบิดจึงจะทำงาน !  
เมื่อระเบิดทำงานแล้ว วัตถุต่างๆในระยะจะถูกทำลาย  
![](https://github.com/compro-itkmitl/Bomberman/blob/master/picreadme/Destroy.png)  
ระวังอย่าให้ตัวละครไปโดนระเบิดนะครับ ไม่เช่นนั้นจะเป็นแบบนี้ Σ(ﾟДﾟ)  
![](https://github.com/compro-itkmitl/Bomberman/blob/master/picreadme/Burn.png)

ในการจบเกมนั้น ผู้เล่นจะต้องทำการสังหารผู้เล่นคนอื่นด้วยระเบิด จนเหลือแค่ตัวผู้เล่นคนเดียวเพื่อจบเกม และชนะได้ ! ( ⊙‿⊙)  
  
![เย่ๆๆๆ](https://github.com/compro-itkmitl/Bomberman/blob/master/picreadme/Win.png)

# ![](https://github.com/compro-itkmitl/Bomberman/blob/master/picreadme/Human.png)Team Members
|  | ชื่อ | นามสกุล | GitHub Username | รหัสนักศึกษา |
|--|--|--|--|--|
| ![](https://github.com/compro-itkmitl/Bomberman/blob/master/picreadme/Net.png) | พิชญพล | เพชรบุญช่วย | [@TheNlesh](https://github.com/TheNlesh) | 60070062 |
| ![](https://github.com/compro-itkmitl/Bomberman/blob/master/picreadme/Coke.png) | ฟุ้งเกียรติ | เผด็จตะคุ | [@CokeFung](https://github.com/CokeFung) | 60070069 |
| ![](https://github.com/compro-itkmitl/Bomberman/blob/master/picreadme/Yut.png) | ยุทธนา | ศรีทำบุญ | [@SriYUTHA](https://github.com/SriYUTHA) | 60070078|
# ![](https://github.com/compro-itkmitl/Bomberman/blob/master/picreadme/Supervisor.png)Assistant Teachers
| ผศ. ดร. กิติ์สุชาต พสุภา | ผศ. ดร. ปานวิทย์ ธุวะนุติ |
|--|--|
| ![](https://github.com/compro-itkmitl/Bomberman/blob/master/picreadme/AjOng.jpg) | ![](https://github.com/compro-itkmitl/Bomberman/blob/master/picreadme/AjPanwit.jpg) |

รายงานนี้เป็นส่วนหนึ่งของวิชา Computer Programming (รหัสวิชา 06016206)

หลักสูตรวิทยาศาสตร์บัณฑิต สาขาวิชาเทคโนโลยีสารสนเทศ

คณะเทคโนโลยีสารสนเทศ สถาบันเทคโนโลยีพระจอมเกล้าเจ้าคุณทหารลาดกระบัง
