#include "gmock/gmock.h"
#include "ik/node.h"
#include "ik/cpputils.hpp"

#define NAME node

using namespace ::testing;

class NAME : public Test
{
public:
    virtual void SetUp() override
    {
    }

    virtual void TearDown() override
    {
    }
};

#ifdef DEBUG
TEST_F(NAME, debug_warn_if_node_with_same_guid_is_added)
#else
TEST_F(NAME, ndebug_dont_warn_if_node_with_same_guid_is_added)
#endif
{
    //internal::CaptureStdout();
    ik::Ref<ik_node> n1 = ik_node_create();
    ik::Ref<ik_node> n2 = ik_node_create_child(n1);
    ik::Ref<ik_node> n3 = ik_node_create_child(n2);
    /*
#ifdef DEBUG
    EXPECT_THAT(internal::GetCapturedStdout().c_str(), StrNe(""));
#else
    EXPECT_THAT(internal::GetCapturedStdout().c_str(), StrEq(""));
#endif*/
}

TEST_F(NAME, find_includes_root_node)
{
    ik::Ref<ik_node> n1 = ik_node_create();           n1->user_data = (void*)1;
    ik::Ref<ik_node> n2 = ik_node_create_child(n1);   n2->user_data = (void*)2;
    ik::Ref<ik_node> n3 = ik_node_create_child(n2);   n3->user_data = (void*)3;
    EXPECT_THAT(ik_node_find(n1, (void*)1), Eq(n1));
}

TEST_F(NAME, find_child_from_root)
{
    ik::Ref<ik_node> n1 = ik_node_create();           n1->user_data = (void*)1;
    ik::Ref<ik_node> n2 = ik_node_create_child(n1);   n2->user_data = (void*)2;
    ik::Ref<ik_node> n3 = ik_node_create_child(n2);   n3->user_data = (void*)3;
    ik::Ref<ik_node> n4 = ik_node_create_child(n2);   n4->user_data = (void*)4;
    EXPECT_THAT(ik_node_find(n1, (void*)4), Eq(n4));
}

TEST_F(NAME, find_doesnt_find_parents)
{
    /*
     *         n1
     *        /  \
     *       n2  n4
     *      /
     *     n3
     */
    ik::Ref<ik_node> n1 = ik_node_create();           n1->user_data = (void*)1;
    ik::Ref<ik_node> n2 = ik_node_create_child(n1);   n2->user_data = (void*)2;
    ik::Ref<ik_node> n3 = ik_node_create_child(n2);   n3->user_data = (void*)3;
    ik::Ref<ik_node> n4 = ik_node_create_child(n1);   n4->user_data = (void*)4;
    EXPECT_THAT(ik_node_find(n1, (void*)4), Eq(n4));
    EXPECT_THAT(ik_node_find(n2, (void*)4), IsNull());
}

TEST_F(NAME, reparent_nodes_works)
{
    ik::Ref<ik_node> n1 = ik_node_create();
    ik::Ref<ik_node> n2 = ik_node_create_child(n1);
    ik::Ref<ik_node> n3 = ik_node_create_child(n2);
    ik::Ref<ik_node> n4 = ik_node_create_child(n1);
    EXPECT_THAT(ik_node_child_count(n1), Eq(2u));
    EXPECT_THAT(ik_node_child_count(n2), Eq(1u));
    ik_node_link(n1, n3);
    EXPECT_THAT(ik_node_child_count(n1), Eq(3u));
    EXPECT_THAT(ik_node_child_count(n2), Eq(0u));
}

TEST_F(NAME, unlink_node_works)
{
    ik::Ref<ik_node> n1 = ik_node_create();
    ik::Ref<ik_node> n2 = ik_node_create_child(n1);
    EXPECT_THAT(ik_node_child_count(n1), Eq(1u));
    ik_node_unlink(n2);
    EXPECT_THAT(ik_node_child_count(n1), Eq(0u));
}

TEST_F(NAME, check_reparent_to_self)
{
    ik::Ref<ik_node> n1 = ik_node_create();
    EXPECT_THAT(ik_node_can_link(n1, n1), IsFalse());
}

TEST_F(NAME, check_circular_dependency)
{
    ik::Ref<ik_node> n1 = ik_node_create();
    ik::Ref<ik_node> n2 = ik_node_create_child(n1);
    ik::Ref<ik_node> n3 = ik_node_create_child(n2);

    EXPECT_THAT(ik_node_can_link(n1, n3), IsTrue());
    EXPECT_THAT(ik_node_can_link(n3, n1), IsFalse());
}
