<?xml version="1.0" encoding="UTF-8"?>
<tileset name="hero" tilewidth="32" tileheight="32" tilecount="85" columns="5">
 <image source="hero_cloth.png" trans="ff24ff" width="160" height="544"/>
 <tile id="0">
  <properties>
   <property name="state" value="attack_r"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="12" y="8" width="11" height="19"/>
  </objectgroup>
  <animation>
   <frame tileid="0" duration="100"/>
   <frame tileid="1" duration="100"/>
   <frame tileid="4" duration="100"/>
  </animation>
 </tile>
 <tile id="5">
  <properties>
   <property name="state" value="walk_r"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="13" y="17" width="7" height="9"/>
  </objectgroup>
  <animation>
   <frame tileid="5" duration="100"/>
   <frame tileid="6" duration="100"/>
   <frame tileid="5" duration="100"/>
   <frame tileid="8" duration="100"/>
  </animation>
 </tile>
 <tile id="10">
  <properties>
   <property name="state" value="idle_r"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="12" y="18" width="8" height="7"/>
  </objectgroup>
  <animation>
   <frame tileid="5" duration="100"/>
   <frame tileid="11" duration="100"/>
   <frame tileid="5" duration="100"/>
   <frame tileid="5" duration="100"/>
   <frame tileid="5" duration="100"/>
  </animation>
 </tile>
 <tile id="13">
  <properties>
   <property name="state" value="pull_r"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="10" y="18" width="13" height="9"/>
  </objectgroup>
  <animation>
   <frame tileid="13" duration="100"/>
   <frame tileid="14" duration="100"/>
  </animation>
 </tile>
 <tile id="15">
  <properties>
   <property name="state" value="attack_up"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="9" y="7" width="15" height="18"/>
  </objectgroup>
  <animation>
   <frame tileid="15" duration="100"/>
   <frame tileid="16" duration="100"/>
   <frame tileid="19" duration="100"/>
  </animation>
 </tile>
 <tile id="20">
  <properties>
   <property name="state" value="walk_up"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="12" y="16.4659" width="8" height="8.5341"/>
  </objectgroup>
  <animation>
   <frame tileid="20" duration="250"/>
   <frame tileid="21" duration="250"/>
   <frame tileid="20" duration="250"/>
   <frame tileid="23" duration="250"/>
  </animation>
 </tile>
 <tile id="25">
  <properties>
   <property name="state" value="idle_up"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="13" y="19" width="6" height="7"/>
  </objectgroup>
  <animation>
   <frame tileid="26" duration="100"/>
   <frame tileid="26" duration="100"/>
   <frame tileid="26" duration="100"/>
   <frame tileid="26" duration="100"/>
   <frame tileid="20" duration="100"/>
  </animation>
 </tile>
 <tile id="27">
  <properties>
   <property name="state" value="pull_up"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="10" y="19" width="12" height="7"/>
  </objectgroup>
  <animation>
   <frame tileid="26" duration="100"/>
   <frame tileid="26" duration="100"/>
   <frame tileid="27" duration="100"/>
   <frame tileid="27" duration="100"/>
  </animation>
 </tile>
 <tile id="30">
  <properties>
   <property name="state" value="attack_dn"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="9" y="11" width="15" height="15"/>
  </objectgroup>
  <animation>
   <frame tileid="30" duration="100"/>
   <frame tileid="31" duration="100"/>
   <frame tileid="34" duration="100"/>
  </animation>
 </tile>
 <tile id="35">
  <properties>
   <property name="state" value="walk_dn"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="12" y="16.6932" width="8" height="8.92045"/>
  </objectgroup>
  <animation>
   <frame tileid="35" duration="250"/>
   <frame tileid="36" duration="250"/>
   <frame tileid="35" duration="250"/>
   <frame tileid="38" duration="250"/>
  </animation>
 </tile>
 <tile id="40">
  <properties>
   <property name="state" value="idle_dn"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="12" y="17" width="8" height="10"/>
  </objectgroup>
  <animation>
   <frame tileid="35" duration="133"/>
   <frame tileid="41" duration="133"/>
   <frame tileid="41" duration="133"/>
   <frame tileid="41" duration="133"/>
  </animation>
 </tile>
 <tile id="43">
  <properties>
   <property name="state" value="pull_dn"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="10" y="18" width="12" height="9"/>
  </objectgroup>
  <animation>
   <frame tileid="43" duration="100"/>
   <frame tileid="43" duration="100"/>
   <frame tileid="41" duration="100"/>
   <frame tileid="41" duration="100"/>
  </animation>
 </tile>
 <tile id="45">
  <objectgroup draworder="index">
   <object id="1" x="10.75" y="15.125" width="10.375" height="11.375"/>
  </objectgroup>
 </tile>
 <tile id="50">
  <properties>
   <property name="state" value="attack_l"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="9" y="16" width="11.125" height="10.5"/>
  </objectgroup>
  <animation>
   <frame tileid="50" duration="100"/>
   <frame tileid="51" duration="100"/>
   <frame tileid="54" duration="100"/>
  </animation>
 </tile>
 <tile id="55">
  <properties>
   <property name="state" value="walk_l"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="12" y="18" width="7" height="6"/>
  </objectgroup>
  <animation>
   <frame tileid="55" duration="100"/>
   <frame tileid="56" duration="100"/>
   <frame tileid="55" duration="100"/>
   <frame tileid="58" duration="100"/>
  </animation>
 </tile>
 <tile id="60">
  <properties>
   <property name="state" value="idle_l"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="12" y="18" width="8" height="6"/>
  </objectgroup>
  <animation>
   <frame tileid="55" duration="100"/>
   <frame tileid="55" duration="100"/>
   <frame tileid="55" duration="100"/>
   <frame tileid="61" duration="100"/>
  </animation>
 </tile>
 <tile id="65">
  <properties>
   <property name="state" value="pull_l"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="6" y="18" width="15" height="10"/>
  </objectgroup>
  <animation>
   <frame tileid="65" duration="100"/>
   <frame tileid="66" duration="100"/>
  </animation>
 </tile>
 <tile id="70">
  <properties>
   <property name="state" value="receive"/>
  </properties>
 </tile>
 <tile id="71">
  <properties>
   <property name="state" value="falling"/>
  </properties>
  <animation>
   <frame tileid="71" duration="100"/>
   <frame tileid="72" duration="100"/>
   <frame tileid="73" duration="100"/>
   <frame tileid="73" duration="100"/>
   <frame tileid="73" duration="100"/>
  </animation>
 </tile>
</tileset>
