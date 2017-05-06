<?xml version="1.0" encoding="UTF-8"?>
<tileset name="sword" tilewidth="32" tileheight="32" tilecount="85" columns="5">
 <image source="sword.png" trans="ff00da" width="160" height="544"/>
 <tile id="0">
  <properties>
   <property name="state" value="stick_r"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="20" y="4" width="10" height="24"/>
  </objectgroup>
  <animation>
   <frame tileid="0" duration="100"/>
   <frame tileid="1" duration="100"/>
   <frame tileid="2" duration="100"/>
  </animation>
 </tile>
 <tile id="5">
  <properties>
   <property name="state" value="stick_up"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="2" y="1" width="28" height="14"/>
  </objectgroup>
  <animation>
   <frame tileid="5" duration="100"/>
   <frame tileid="6" duration="100"/>
   <frame tileid="7" duration="100"/>
  </animation>
 </tile>
 <tile id="10">
  <properties>
   <property name="state" value="stick_dn"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="2" y="19" width="27" height="10"/>
  </objectgroup>
  <animation>
   <frame tileid="10" duration="100"/>
   <frame tileid="11" duration="100"/>
   <frame tileid="12" duration="100"/>
  </animation>
 </tile>
 <tile id="15">
  <properties>
   <property name="state" value="stick_l"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="2" y="5" width="10" height="24"/>
  </objectgroup>
  <animation>
   <frame tileid="15" duration="100"/>
   <frame tileid="16" duration="100"/>
   <frame tileid="17" duration="100"/>
  </animation>
 </tile>
 <tile id="20">
  <properties>
   <property name="state" value="sword_r"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="21" y="3" width="7" height="26"/>
  </objectgroup>
  <animation>
   <frame tileid="20" duration="100"/>
   <frame tileid="21" duration="100"/>
   <frame tileid="22" duration="100"/>
  </animation>
 </tile>
 <tile id="25">
  <properties>
   <property name="state" value="sword_up"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="2" y="1" width="27" height="14"/>
  </objectgroup>
  <animation>
   <frame tileid="25" duration="100"/>
   <frame tileid="26" duration="100"/>
   <frame tileid="27" duration="100"/>
  </animation>
 </tile>
 <tile id="30">
  <properties>
   <property name="state" value="sword_dn"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="3" y="20" width="26" height="11"/>
  </objectgroup>
  <animation>
   <frame tileid="30" duration="100"/>
   <frame tileid="31" duration="100"/>
   <frame tileid="32" duration="100"/>
  </animation>
 </tile>
 <tile id="35">
  <properties>
   <property name="state" value="sword_l"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="2" y="2" width="10" height="28"/>
  </objectgroup>
  <animation>
   <frame tileid="35" duration="100"/>
   <frame tileid="36" duration="100"/>
   <frame tileid="37" duration="100"/>
  </animation>
 </tile>
 <tile id="50">
  <objectgroup draworder="index">
   <object id="1" x="3" y="3" width="9" height="26"/>
  </objectgroup>
 </tile>
 <tile id="55">
  <objectgroup draworder="index">
   <object id="1" x="3" y="3" width="8" height="26"/>
  </objectgroup>
 </tile>
</tileset>
