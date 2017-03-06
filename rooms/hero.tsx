<?xml version="1.0" encoding="UTF-8"?>
<tileset name="hero" tilewidth="32" tileheight="32" tilecount="85" columns="5">
 <image source="hero_cloth.png" trans="ff24ff" width="160" height="544"/>
 <tile id="0">
  <properties>
   <property name="state" value="attack_r"/>
  </properties>
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
   <object id="1" x="10.625" y="17.5" width="11.375" height="7.28409"/>
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
  <animation>
   <frame tileid="5" duration="100"/>
   <frame tileid="11" duration="300"/>
  </animation>
 </tile>
 <tile id="15">
  <properties>
   <property name="state" value="attack_up"/>
  </properties>
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
   <object id="1" x="10.25" y="16.4659" width="11.75" height="9.53409"/>
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
  <animation>
   <frame tileid="20" duration="100"/>
   <frame tileid="26" duration="100"/>
  </animation>
 </tile>
 <tile id="30">
  <properties>
   <property name="state" value="attack_dn"/>
  </properties>
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
   <object id="1" x="10.125" y="16.6932" width="11.75" height="8.92045"/>
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
   <object id="1" x="9.875" y="16.75" width="12.25" height="10.5"/>
  </objectgroup>
  <animation>
   <frame tileid="35" duration="250"/>
   <frame tileid="41" duration="250"/>
   <frame tileid="41" duration="250"/>
   <frame tileid="41" duration="250"/>
  </animation>
 </tile>
 <tile id="45">
  <properties>
   <property name="state" value="walk_l"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="10.75" y="15.125" width="10.375" height="11.375"/>
  </objectgroup>
  <animation>
   <frame tileid="45" duration="100"/>
   <frame tileid="46" duration="100"/>
   <frame tileid="45" duration="100"/>
   <frame tileid="48" duration="100"/>
  </animation>
 </tile>
 <tile id="50">
  <properties>
   <property name="state" value="pull_l"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="9.125" y="17.875" width="11.125" height="10.5"/>
  </objectgroup>
  <animation>
   <frame tileid="50" duration="250"/>
   <frame tileid="51" duration="250"/>
  </animation>
 </tile>
 <tile id="55">
  <properties>
   <property name="state" value="attack_l"/>
  </properties>
  <animation>
   <frame tileid="55" duration="100"/>
   <frame tileid="56" duration="100"/>
   <frame tileid="59" duration="100"/>
  </animation>
 </tile>
 <tile id="60">
  <properties>
   <property name="state" value="idle_l"/>
  </properties>
  <animation>
   <frame tileid="45" duration="100"/>
   <frame tileid="61" duration="100"/>
  </animation>
 </tile>
</tileset>
