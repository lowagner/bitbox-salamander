<?xml version="1.0" encoding="UTF-8"?>
<tileset name="rat" tilewidth="22" tileheight="22" tilecount="32" columns="4">
 <image source="rat.png" trans="ff00da" width="88" height="176"/>
 <tile id="0">
  <properties>
   <property name="state" value="walk_r"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="7" y="7" width="13" height="8"/>
  </objectgroup>
  <animation>
   <frame tileid="0" duration="100"/>
   <frame tileid="1" duration="100"/>
   <frame tileid="2" duration="100"/>
  </animation>
 </tile>
 <tile id="4">
  <properties>
   <property name="state" value="walk_l"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="0" y="6" width="15" height="8"/>
  </objectgroup>
  <animation>
   <frame tileid="4" duration="100"/>
   <frame tileid="5" duration="100"/>
   <frame tileid="6" duration="100"/>
  </animation>
 </tile>
 <tile id="8">
  <properties>
   <property name="state" value="walk_u"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="7" y="2" width="7" height="12"/>
  </objectgroup>
  <animation>
   <frame tileid="8" duration="100"/>
   <frame tileid="9" duration="100"/>
   <frame tileid="10" duration="100"/>
   <frame tileid="11" duration="100"/>
  </animation>
 </tile>
 <tile id="12">
  <properties>
   <property name="state" value="walk_d"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="5" y="8" width="10" height="14"/>
  </objectgroup>
  <animation>
   <frame tileid="12" duration="100"/>
   <frame tileid="13" duration="100"/>
   <frame tileid="14" duration="100"/>
   <frame tileid="15" duration="100"/>
  </animation>
 </tile>
 <tile id="16">
  <properties>
   <property name="state" value="pause_r"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="6" y="6" width="14" height="10"/>
  </objectgroup>
  <animation>
   <frame tileid="16" duration="100"/>
   <frame tileid="16" duration="100"/>
   <frame tileid="22" duration="100"/>
  </animation>
 </tile>
 <tile id="17">
  <properties>
   <property name="state" value="pause_l"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="-1" y="7" width="16" height="9"/>
  </objectgroup>
  <animation>
   <frame tileid="17" duration="100"/>
   <frame tileid="17" duration="100"/>
   <frame tileid="23" duration="100"/>
  </animation>
 </tile>
 <tile id="18">
  <properties>
   <property name="state" value="pause_u"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="6" y="1" width="9" height="11"/>
  </objectgroup>
 </tile>
 <tile id="19">
  <properties>
   <property name="state" value="pause_d"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="5" y="8" width="9" height="12"/>
  </objectgroup>
 </tile>
 <tile id="20">
  <properties>
   <property name="state" value="killed"/>
  </properties>
  <animation>
   <frame tileid="20" duration="100"/>
   <frame tileid="21" duration="100"/>
  </animation>
 </tile>
 <tile id="24">
  <properties>
   <property name="state" value="sleeping"/>
  </properties>
  <animation>
   <frame tileid="24" duration="100"/>
   <frame tileid="24" duration="100"/>
   <frame tileid="25" duration="100"/>
   <frame tileid="25" duration="100"/>
   <frame tileid="26" duration="100"/>
   <frame tileid="26" duration="100"/>
   <frame tileid="25" duration="100"/>
   <frame tileid="25" duration="100"/>
  </animation>
 </tile>
</tileset>
