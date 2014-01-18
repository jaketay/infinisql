/*
 * Copyright (c) 2013 Mark Travis <mtravis15432+src@gmail.com>
 * All rights reserved. No warranty, explicit or implicit, provided.
 *
 * This file is part of InfiniSQL(tm).
 
 * InfiniSQL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3
 * as published by the Free Software Foundation.
 *
 * InfiniSQL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with InfiniSQL. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file   Catalog.h
 * @author Mark Travis <mtravis15432+src@gmail.com>
 * @date   Mon Jan 13 08:05:51 2014
 * 
 * @brief  catalog is a collection of schemata and users
 */

#ifndef INFINISQLCATALOG_H
#define INFINISQLCATALOG_H

#include "Metadata.h"

class User;
class Schema;
class Table;
class Index;

class Catalog : public Metadata
{
public:
    Catalog();
    /** 
     * @brief first instantiatiation from UserSchemaMgr
     *
     * UserSchemaMgr needs to make sure there's no duplicate name
     * or id, and to populate its catalog maps
     *
     * @param idarg catalogid (generated by UserSchemaMgr)
     * @param namearg name
     */
    Catalog(int16_t idarg, std::string namearg);
    Catalog(const Catalog &orig);
    Catalog &operator= (const Catalog &orig);
    /** 
     * @brief copy sufficient for reproduction elsewhere
     *
     * requires post-processing for destination actors' pointers to related
     * objects
     *
     * @param orig 
     */
    void cp(const Catalog &orig);
    
    ~Catalog();

    void ser(Serdes &output);
    size_t sersize();
    void des(Serdes &input);
    
    /** 
     * @brief userid generator
     *
     *
     * @return next userid
     */
    int16_t getnextuserid();
    /** 
     * @brief schemaid generator
     *
     *
     * @return next schemaid
     */
    int16_t getnextschemaid();
    /** 
     * @brief tableid generator
     *
     *
     * @return next tableid
     */
    int16_t getnexttableid();
    /** 
     * @brief indexid generator
     *
     *
     * @return next indexid
     */
    int16_t getnextindexid();

    /** 
     * @brief create and open LMDB environment
     *
     * @param path filesystem path for environment
     *
     * @return 0 for success, otherwise result from mdb_env_create() or
     * mdb_env_open()
     */
    int openEnvironment(std::string path);
    /** 
     * @brief close LMDB environment
     *
     * from the FM: "All transactions, databases, and cursors must already be
     * closed before calling this function. Attempts to use any such handles
     * after calling this function will cause a SIGSEGV."
     */
    void closeEnvironment();
    /** 
     * @brief delete data and lock files in LMDB environment directory
     *
     * the environment must not be open
     *
     * @param path filesystem path of LMDB environment
     *
     * @return status from remove() for either of the files
     */
    int deleteEnvironment(std::string path);
    
    int16_t nextuserid;
    int16_t nextschemaid;
    int16_t nexttableid;
    int16_t nextindexid;

    boost::unordered_map<std::string, int16_t> userName2Id; /**< userName2Id[name]=userid */
    boost::unordered_map<int16_t, User *> userid2User; /**< userid2User[userid]=User* */
    boost::unordered_map<std::string, int16_t> schemaName2Id; /**< schemaName2Id[name]=schemaid */
    boost::unordered_map<int16_t, Schema *> schemaid2Schema; /**< schemaid2Schema[schemaid]=Schema* */
    boost::unordered_map<std::string, int16_t> tableName2Id; /**< tableName2Id[name]=tableid */
    boost::unordered_map<int16_t, Table *> tableid2Table; /**< tableid2Table[tableid]=Table* */
    boost::unordered_map<std::string, int16_t> indexName2Id; /**< indexName2Id[name]=indexid */
    boost::unordered_map<int16_t, Index *> indexid2Index; /**< indexid2Index[indexid]=Index* */
};

#endif // INFINISQLCATALOG_H
