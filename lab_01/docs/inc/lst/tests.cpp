#pragma once
#include "distance.hpp"
#include <vector>
#include <gtest/gtest.h>

TEST(levenshteinDistanceSimple, identical_strings)
{
    std::string s1 = "hello";
    std::string s2 = "hello";

    int lev_dst_simple = LevenshteinDistanceSimple(s1, s2, false);
    EXPECT_EQ(lev_dst_simple, 0);
}

TEST(levenshteinDistance, identical_strings)
{
    std::string s1 = "hello";
    std::string s2 = "hello";

    int lev_dst = LevenshteinDistance(s1, s2);
    EXPECT_EQ(lev_dst, 0);
}

TEST(levenshteinDistanceRecursive, identical_strings)
{
    std::string s1 = "hello";
    std::string s2 = "hello";

    int lev_dst_rec = LevenshteinDistanceRecursive(s1, s2, s1.size(), s2.size());
    EXPECT_EQ(lev_dst_rec, 0);
}

TEST(levenshteinDistanceRecursiveCache, identical_strings)
{
    std::string s1 = "hello";
    std::string s2 = "hello";

    std::vector<std::vector<int>> cache;
    InitLDRCache(cache, s1.size() + 1, s2.size() + 1);
    int lev_dst_rec_cache = LevenshteinDistanceRecursiveCache(s1, s2, s1.size(), s2.size(), cache);
    EXPECT_EQ(lev_dst_rec_cache, 0);
}

TEST(damerauLevenshteinDistanceSimple, identical_strings)
{
    std::string s1 = "hello";
    std::string s2 = "hello";
    
    int dam_lev = DamerauLevenshteinDistanceSimple(s1, s2);
    EXPECT_EQ(dam_lev, 0);
}

TEST(levenshteinDistanceSimple, not_even_partly_simular)
{
    std::string s1 = "meoww";
    std::string s2 = "hello";

    int lev_dst_simple = LevenshteinDistanceSimple(s1, s2, false);
    EXPECT_EQ(lev_dst_simple, 4);
}

TEST(levenshteinDistance, not_even_partly_simular)
{
    std::string s1 = "meoww";
    std::string s2 = "hello";

    int lev_dst = LevenshteinDistance(s1, s2);
    EXPECT_EQ(lev_dst, 4);
}

TEST(levenshteinDistanceRecursive, not_even_partly_simular)
{
    std::string s1 = "meoww";
    std::string s2 = "hello";

    int lev_dst_rec = LevenshteinDistanceRecursive(s1, s2, s1.size(), s2.size());
    EXPECT_EQ(lev_dst_rec, 4);
}

TEST(levenshteinDistanceRecursiveCache, not_even_partly_simular)
{
    std::string s1 = "meoww";
    std::string s2 = "hello";

    std::vector<std::vector<int>> cache;
    InitLDRCache(cache, s1.size() + 1, s2.size() + 1);
    int lev_dst_rec_cache = LevenshteinDistanceRecursiveCache(s1, s2, s1.size(), s2.size(), cache);
    EXPECT_EQ(lev_dst_rec_cache, 4);
}

TEST(damerauLevenshteinDistanceSimple, not_even_partly_simular)
{
    std::string s1 = "meoww";
    std::string s2 = "hello";

    int dam_lev = DamerauLevenshteinDistanceSimple(s1, s2);
    EXPECT_EQ(dam_lev, 4);
}


TEST(levenshteinDistanceSimple, different_strings)
{
    std::string s1 = "ppppp";
    std::string s2 = "hello";

    int lev_dst_simple = LevenshteinDistanceSimple(s1, s2, false);
    EXPECT_EQ(lev_dst_simple, 5);
}

TEST(levenshteinDistance, different_strings)
{
    std::string s1 = "ppppp";
    std::string s2 = "hello";

    int lev_dst = LevenshteinDistance(s1, s2);
    EXPECT_EQ(lev_dst, 5);
}

TEST(levenshteinDistanceRecursive, different_strings)
{
    std::string s1 = "ppppp";
    std::string s2 = "hello";

    int lev_dst_rec = LevenshteinDistanceRecursive(s1, s2, s1.size(), s2.size());
    EXPECT_EQ(lev_dst_rec, 5);
}

TEST(levenshteinDistanceRecursiveCache, different_strings)
{
    std::string s1 = "ppppp";
    std::string s2 = "hello";

    std::vector<std::vector<int>> cache;
    InitLDRCache(cache, s1.size() + 1, s2.size() + 1);
    int lev_dst_rec_cache = LevenshteinDistanceRecursiveCache(s1, s2, s1.size(), s2.size(), cache);
    EXPECT_EQ(lev_dst_rec_cache, 5);
}

TEST(damerauLevenshteinDistanceSimple, different_strings)
{
    std::string s1 = "ppppp";
    std::string s2 = "hello";

    int dam_lev = DamerauLevenshteinDistanceSimple(s1, s2);
    EXPECT_EQ(dam_lev, 5);
}

TEST(levenshteinDistanceSimple, t_solve)
{
    std::string s1 = "helol";
    std::string s2 = "hello";

    int lev_dst_simple = LevenshteinDistanceSimple(s1, s2, false);
    EXPECT_EQ(lev_dst_simple, 2);
}

TEST(levenshteinDistance, t_solve)
{
    std::string s1 = "helol";
    std::string s2 = "hello";

    int lev_dst = LevenshteinDistance(s1, s2);
    EXPECT_EQ(lev_dst, 2);
}

TEST(levenshteinDistanceRecursive, t_solve)
{
    std::string s1 = "helol";
    std::string s2 = "hello";

    int lev_dst_rec = LevenshteinDistanceRecursive(s1, s2, s1.size(), s2.size());
    EXPECT_EQ(lev_dst_rec, 2);
}

TEST(levenshteinDistanceRecursiveCache, t_solve)
{
    std::string s1 = "helol";
    std::string s2 = "hello";

    std::vector<std::vector<int>> cache;
    InitLDRCache(cache, s1.size() + 1, s2.size() + 1);
    int lev_dst_rec_cache = LevenshteinDistanceRecursiveCache(s1, s2, s1.size(), s2.size(), cache);
    EXPECT_EQ(lev_dst_rec_cache, 2);
}

TEST(damerauLevenshteinDistanceSimple, t_solve)
{
    std::string s1 = "helol";
    std::string s2 = "hello";

    int dam_lev = DamerauLevenshteinDistanceSimple(s1, s2);
    EXPECT_EQ(dam_lev, 1);
}
