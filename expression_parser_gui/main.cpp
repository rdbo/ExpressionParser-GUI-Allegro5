#include "pch.h"
#include "expr_parser.h"

ALLEGRO_DISPLAY* display;
ALLEGRO_EVENT_QUEUE* queue;
Window wrect;
ImGuiWindowFlags wf = 0;
ImFont* roboto;
ImFont* roboto2;

char expr[MAX_OPERATIONS][DEFAULT_MAX];
int intIn = 0;
char hexOut[DEFAULT_MAX] = "0x0";
int decimals[MAX_OPERATIONS];
int tdecimals[MAX_OPERATIONS];
double output[MAX_OPERATIONS];
char buffer[MAX_OPERATIONS][10];
int id = DEFAULT_ID;
int operations = 1;

std::string CreateNewLabel(const char* label);
void ResetID();
void ResetOperation(int i);

static te_variable vars[] = 
{
	{"root", PMath::root, TE_FUNCTION2}
};

void ExpressionParser::Solve(int i)
{
    te_expr* n = te_compile(expr[i], vars, 1, nullptr);
    output[i] = te_eval(n);
    te_free(n);
	snprintf(buffer[i], sizeof(buffer[i]), "%s%i%s", "%.", decimals[i], "f");
}

void ExpressionParser::Draw()
{
    ImGui::Begin("ExpressionParser", 0, wf);

	ImGui::PushFont(roboto2);
	ImGui::TextColored(COLOR_BLUE, "ExpressionParser");
	ImGui::PopFont();
	ImGui::Separator();
	ImGui::PushFont(roboto);
	ImGui::Text("Convert to Hexadecimal");
	ImGui::Text("Input: "); ImGui::SameLine();
	if (ImGui::InputInt(CreateNewLabel("").c_str(), &intIn))
	{
		snprintf(hexOut, sizeof(hexOut), "0x%x", intIn);
		for (int i = 0; i < sizeof(hexOut); i++)
		{
			if (!isupper(hexOut[i]) && hexOut[i] != 'x')
			{
				hexOut[i] = toupper(hexOut[i]);
			}
		}
	}

	ImGui::Text("Output: "); ImGui::SameLine();
	ImGui::InputText(CreateNewLabel("").c_str(), hexOut, sizeof(hexOut), ImGuiInputTextFlags_ReadOnly);
	ImGui::Separator();
	for(int i = 1; i <= operations; i++)
	{
		ImGui::Text("Enter your expression: ");
		if (ImGui::InputText(CreateNewLabel("").c_str(), expr[i], sizeof(expr[i]), ImGuiInputTextFlags_EnterReturnsTrue))
			ExpressionParser::Solve(i);
		if (ImGui::Button(CreateNewLabel("Parse").c_str()))
			Solve(i);
		ImGui::Text("Output: "); ImGui::SameLine();
		ImGui::InputDouble(CreateNewLabel("").c_str(), &output[i], 0, 0, buffer[i], ImGuiInputTextFlags_ReadOnly);
		ImGui::Text("Decimals: "); ImGui::SameLine();
		if (ImGui::InputInt(CreateNewLabel("").c_str(), &tdecimals[i]))
		{
			if (tdecimals[i] > MAX_DECIMALS)
				tdecimals[i] = MAX_DECIMALS;

			else if (tdecimals[i] < 0)
				tdecimals[i] = 0;

			decimals[i] = tdecimals[i];
			snprintf(buffer[i], sizeof(buffer[i]), "%s%i%s", "%.", decimals[i], "f");
		}
		ImGui::Separator();
	}
	ImGui::Text("Add/Remove operations"); ImGui::SameLine();
	if(ImGui::Button(CreateNewLabel("+").c_str(), BTN_VEC))
		operations + 1 <= MAX_OPERATIONS ? operations += 1 : 0;
	ImGui::SameLine();
	if (ImGui::Button(CreateNewLabel("-").c_str(), BTN_VEC))
	{
		ResetOperation(operations);
		operations - 1 >= 0 ? operations -= 1 : 0;
	}
	ResetID();
	ImGui::PopFont();
	ImGui::End();
}

int main()
{
    ExpressionParser::Init();
    return 0;
}

std::string CreateNewLabel(const char* label)
{
    id += 1;
	std::string s;
	s = label;
	s += "##";
	s += std::to_string(id);
	return s;
}

void ResetID()
{
    id = DEFAULT_ID;
}

void ResetOperation(int i)
{
	memset(expr[i], {}, sizeof(expr[i]));
	decimals[i] = 0;
	tdecimals[i] = 0;
	output[i] = 0;
	memset(buffer[i], {}, sizeof(buffer[i]));
}