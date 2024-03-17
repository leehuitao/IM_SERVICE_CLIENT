/*
 Navicat Premium Data Transfer

 Source Server         : 127.0.0.1
 Source Server Type    : MySQL
 Source Server Version : 50143
 Source Host           : localhost:3306
 Source Schema         : test

 Target Server Type    : MySQL
 Target Server Version : 50143
 File Encoding         : 65001

 Date: 25/08/2023 11:15:02
*/

SET NAMES utf8;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for dept_info
-- ----------------------------
DROP TABLE IF EXISTS `dept_info`;
CREATE TABLE `dept_info`  (
  `DeptName` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `DeptID` int(11) NULL DEFAULT NULL,
  `ParentDeptID` int(11) NULL DEFAULT NULL,
  `Level` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Compact;

-- ----------------------------
-- Records of dept_info
-- ----------------------------
INSERT INTO `dept_info` VALUES ('Contents1', 1, 0, '1');
INSERT INTO `dept_info` VALUES ('Contents3', 3, 1, '2');
INSERT INTO `dept_info` VALUES ('Contents4', 4, 1, '2');
INSERT INTO `dept_info` VALUES ('Contents5', 5, 2, '2');
INSERT INTO `dept_info` VALUES ('Contents2', 2, 0, '1');

-- ----------------------------
-- Table structure for group_members
-- ----------------------------
DROP TABLE IF EXISTS `group_members`;
CREATE TABLE `group_members`  (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '群组成员ID',
  `group_id` int(11) NULL DEFAULT NULL COMMENT '群组ID',
  `user_id` int(11) NULL DEFAULT NULL COMMENT '用户ID',
  `joined_at` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '加入时间',
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `group_id`(`group_id`) USING BTREE,
  INDEX `user_id`(`user_id`) USING BTREE,
  CONSTRAINT `group_members_ibfk_1` FOREIGN KEY (`group_id`) REFERENCES `groups` (`id`) ON DELETE RESTRICT ON UPDATE RESTRICT,
  CONSTRAINT `group_members_ibfk_2` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`) ON DELETE RESTRICT ON UPDATE RESTRICT
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Compact;

-- ----------------------------
-- Records of group_members
-- ----------------------------

-- ----------------------------
-- Table structure for group_messages
-- ----------------------------
DROP TABLE IF EXISTS `group_messages`;
CREATE TABLE `group_messages`  (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '消息ID',
  `group_id` int(11) NULL DEFAULT NULL COMMENT '群组ID',
  `group_name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '群组',
  `user_id` int(11) NULL DEFAULT NULL COMMENT '发送者用户ID',
  `user_name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '发送者用户',
  `content` text CHARACTER SET utf8 COLLATE utf8_general_ci NULL COMMENT '消息内容',
  `sent_at` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '发送时间',
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `group_id`(`group_id`) USING BTREE,
  INDEX `user_id`(`user_id`) USING BTREE,
  CONSTRAINT `group_messages_ibfk_1` FOREIGN KEY (`group_id`) REFERENCES `groups` (`id`) ON DELETE RESTRICT ON UPDATE RESTRICT,
  CONSTRAINT `group_messages_ibfk_2` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`) ON DELETE RESTRICT ON UPDATE RESTRICT
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Compact;

-- ----------------------------
-- Records of group_messages
-- ----------------------------

-- ----------------------------
-- Table structure for groups
-- ----------------------------
DROP TABLE IF EXISTS `groups`;
CREATE TABLE `groups`  (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '群组ID',
  `group_name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '群名称',
  `announcement` text CHARACTER SET utf8 COLLATE utf8_general_ci NULL COMMENT '群公告内容',
  `created_at` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '创建时间',
  `creator_id` int(11) NULL DEFAULT NULL COMMENT '创建者ID',
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `creator_id`(`creator_id`) USING BTREE,
  CONSTRAINT `groups_ibfk_1` FOREIGN KEY (`creator_id`) REFERENCES `users` (`id`) ON DELETE RESTRICT ON UPDATE RESTRICT
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Compact;

-- ----------------------------
-- Records of groups
-- ----------------------------

-- ----------------------------
-- Table structure for login_logs
-- ----------------------------
DROP TABLE IF EXISTS `login_logs`;
CREATE TABLE `login_logs`  (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '日志ID',
  `user_id` int(11) NULL DEFAULT NULL COMMENT '用户ID',
  `user_name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '用户',
  `login_time` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '登录时间',
  `ip_address` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '登录IP地址',
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `user_id`(`user_id`) USING BTREE,
  CONSTRAINT `login_logs_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`) ON DELETE RESTRICT ON UPDATE RESTRICT
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Compact;

-- ----------------------------
-- Records of login_logs
-- ----------------------------

-- ----------------------------
-- Table structure for personal_chat_messages
-- ----------------------------
DROP TABLE IF EXISTS `personal_chat_messages`;
CREATE TABLE `personal_chat_messages`  (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '消息ID',
  `sender_id` int(11) NULL DEFAULT NULL COMMENT '发送者用户ID',
  `sender_name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '发送者用户',
  `receiver_id` int(11) NULL DEFAULT NULL COMMENT '接收者用户ID',
  `receiver_name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '接收者用户',
  `content` text CHARACTER SET utf8 COLLATE utf8_general_ci NULL COMMENT '消息内容',
  `sent_at` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '发送时间',
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `sender_id`(`sender_id`) USING BTREE,
  INDEX `receiver_id`(`receiver_id`) USING BTREE,
  CONSTRAINT `personal_chat_messages_ibfk_1` FOREIGN KEY (`sender_id`) REFERENCES `users` (`id`) ON DELETE RESTRICT ON UPDATE RESTRICT,
  CONSTRAINT `personal_chat_messages_ibfk_2` FOREIGN KEY (`receiver_id`) REFERENCES `users` (`id`) ON DELETE RESTRICT ON UPDATE RESTRICT
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Compact;

-- ----------------------------
-- Records of personal_chat_messages
-- ----------------------------

-- ----------------------------
-- Table structure for userinfo
-- ----------------------------
DROP TABLE IF EXISTS `userinfo`;
CREATE TABLE `userinfo`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `UserName` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `UserLoginName` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `PassWord` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `Notice` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `MacAddress` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `LoginTime` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `ParentDeptID` int(11) NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 13 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Compact;

-- ----------------------------
-- Records of userinfo
-- ----------------------------
INSERT INTO `userinfo` VALUES (1, 'ceshi1', 'ceshi1', '123456', '1', 'dddddd', '1', 1);
INSERT INTO `userinfo` VALUES (2, 'ceshi2', 'ceshi2', '123456', '1', 'dddddd', '1', 2);
INSERT INTO `userinfo` VALUES (3, 'ceshi3', 'ceshi3', '123456', '1', 'dddddd', '1', 1);
INSERT INTO `userinfo` VALUES (4, 'ceshi4', 'ceshi4', '123456', '1', 'dddddd', '1', 3);
INSERT INTO `userinfo` VALUES (5, 'ceshi5', 'ceshi5', '123456', '1', 'dddddd', '1', 4);
INSERT INTO `userinfo` VALUES (6, 'ceshi6', 'ceshi6', '123456', '1', 'dddddd', '1', 2);
INSERT INTO `userinfo` VALUES (7, 'ceshi7', 'ceshi7', '123456', '1', 'dddddd', '1', 5);
INSERT INTO `userinfo` VALUES (8, 'ceshi8', 'ceshi8', '123456', '1', 'dddddd', '1', 1);
INSERT INTO `userinfo` VALUES (9, 'ceshi9', 'ceshi9', '123456', '1', 'dddddd', '1', 1);
INSERT INTO `userinfo` VALUES (10, 'ceshi10', 'ceshi10', '123456', '1', 'dddddd', '1', 1);
INSERT INTO `userinfo` VALUES (11, 'test', 'test', 'test', '1', 'dddddd', '1', 1);
INSERT INTO `userinfo` VALUES (12, 'test1', 'test1', 'test1', '1', 'dddddd', '1', 1);

-- ----------------------------
-- Table structure for users
-- ----------------------------
DROP TABLE IF EXISTS `users`;
CREATE TABLE `users`  (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '用户ID',
  `username` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '用户名',
  `login_name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '登录名',
  `password` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '密码',
  `last_login` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '上次登录时间',
  `dept_id` int(11) NULL DEFAULT NULL COMMENT '群组ID',
  `permission_level` int(11) NULL DEFAULT NULL COMMENT '权限等级',
  `created_time` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '创建时间',
  `creator_id` int(11) NULL DEFAULT NULL COMMENT '创建者ID',
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `id`(`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 88880010 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Compact;

-- ----------------------------
-- Records of users
-- ----------------------------
INSERT INTO `users` VALUES (88880000, '管理员', 'admin', '123', 1, 1, 1, '2023:8:25-15:19:00', 1);
INSERT INTO `users` VALUES (88880001, '王科雷', 'wkl', '123', 1, 1, 1, '2023:8:23-16:37:00', 1);
INSERT INTO `users` VALUES (88880002, '李惠涛', 'lht', '123', 1, 1, 1, '2023:8:23-16:37:00', 1);
INSERT INTO `users` VALUES (88880003, '章学勇', 'zxy', '123', 1, 2, 1, '2023:8:23-16:37:00', 1);
INSERT INTO `users` VALUES (88880004, '吴展浩', 'wzh', '123', 1, 2, 1, '2023:8:23-16:37:00', 1);
INSERT INTO `users` VALUES (88880005, '甄业鑫', 'zyx', '123', 1, 3, 1, '2023:8:23-16:37:00', 1);
INSERT INTO `users` VALUES (88880006, '王永明', 'wym', '123', 1, 4, 1, '2023:8:23-16:37:00', 1);
INSERT INTO `users` VALUES (88880007, '朱薛', 'zx', '123', 1, 5, 1, '2023:8:23-16:37:00', 1);
INSERT INTO `users` VALUES (88880008, '孙磊', 'sl', '123', 1, 1, 1, '2023:8:24-15:19:00', 1);
INSERT INTO `users` VALUES (88880009, '陈鹏杰', 'cpj', '123', 1, 1, 1, '2023:8:24-15:19:00', 1);

SET FOREIGN_KEY_CHECKS = 1;
